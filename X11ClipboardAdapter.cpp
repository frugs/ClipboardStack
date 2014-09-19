#include <iostream>
#include <vector>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "X11ClipboardAdapter.h"

X11ClipboardAdapter::Property X11ClipboardAdapter::readProperty(Atom property) {
    Atom actualType;
    int actualFormat;
    unsigned long itemCount;
    unsigned long bytesAfter;
    unsigned char *ret=0;

    int readBytes = 1024;

    do {
        if (ret != 0) {
            XFree(ret);
        }

        XGetWindowProperty(_display, _window, property, 0, readBytes, False, AnyPropertyType,
                &actualType, &actualFormat, &itemCount, &bytesAfter, &ret);

        readBytes *= 2;
    } while (bytesAfter != 0);


    Property result = { actualFormat, itemCount, ret };
    return result;
}

X11ClipboardAdapter::X11ClipboardAdapter(Display *display, Window window, X11InternalAtoms x11InternalAtoms, ClipboardStack *clipboardStack) {
    _display = display;
    _window = window;
    _clipboardStack = clipboardStack;

    _x11InternalAtoms = x11InternalAtoms;
    _targetCount = 0;
}

void X11ClipboardAdapter::initialise() {
    Window owner = XGetSelectionOwner(_display, _x11InternalAtoms.getXAClipboardAtom());

    if (owner == None) {
        XSetSelectionOwner(_display, _x11InternalAtoms.getXAClipboardAtom(), _window, CurrentTime);
    } else {
        onCopy();
    }
}

void X11ClipboardAdapter::onCopy() {
    _clipboardStack->push();
    XConvertSelection(_display, _x11InternalAtoms.getXAClipboardAtom(), _x11InternalAtoms.getXATargetsAtom(), _x11InternalAtoms.getXAClipboardAtom(), _window, CurrentTime);
}

void X11ClipboardAdapter::handleEvent(XEvent event) {
    switch (event.type) {
        case SelectionNotify: {
            XSelectionEvent xSelection = event.xselection;
            Property prop = readProperty(_x11InternalAtoms.getXAClipboardAtom());

            if (xSelection.target == _x11InternalAtoms.getXATargetsAtom()) {
                for (int i = 0; i < prop.itemCount; i++) {
                    XConvertSelection(_display, _x11InternalAtoms.getXAClipboardAtom(), ((Atom *) prop.data)[i], _x11InternalAtoms.getXAClipboardAtom(), _window, xSelection.time);
                }
                _targetCount = prop.itemCount;
            } else {
                std::cout << "SelectionNotify->" << XGetAtomName(_display, xSelection.target) << std::endl;

                unsigned long size = prop.itemCount  * prop.format / 8;
                std::vector<unsigned char> data(prop.data, prop.data + size);

                _clipboardStack->addConversion(xSelection.target, data);

                if (_targetCount == _clipboardStack->top()->size()) {
                    XSetSelectionOwner(_display, _x11InternalAtoms.getXAClipboardAtom(), _window, CurrentTime);
                }
            }
            break;
        }
        case SelectionClear:
            std::cout << "SelectionClear" << std::endl;
            onCopy();
            break;
        case SelectionRequest: {
            XSelectionRequestEvent xSelectionRequest = event.xselectionrequest;

            std::cout << "SelectionRequest->" << XGetAtomName(_display, xSelectionRequest.target) << std::endl;

            XSelectionEvent response;
            response.type = SelectionNotify;
            response.serial = (unsigned long) event.xany.send_event;
            response.send_event = True;
            response.display = _display;
            response.requestor = xSelectionRequest.requestor;
            response.selection = xSelectionRequest.selection;
            response.property = xSelectionRequest.property;
            response.target = None;
            response.time = xSelectionRequest.time;

            Atom target = xSelectionRequest.target;
            if (target == _x11InternalAtoms.getXATargetsAtom()) {
                response.target = target;

                std::vector<Atom> possibleTargets;
                for (auto &entry : *_clipboardStack->top()) {
                    possibleTargets.push_back((Atom) entry.first);
                }

                XChangeProperty(_display, xSelectionRequest.requestor,
                        xSelectionRequest.property, XA_ATOM, 32, PropModeReplace,
                        (unsigned char *) &possibleTargets[0], 1);

            } else if (xSelectionRequest.selection == _x11InternalAtoms.getXAClipboardAtom() && _clipboardStack->top()->count(target) > 0) {
                response.target = target;

                std::vector<unsigned char> data = _clipboardStack->top()->at(target);
                XChangeProperty(_display, xSelectionRequest.requestor,
                        xSelectionRequest.property, target, 8,
                        PropModeReplace, &data[0], (int) data.size());
            }
            XSendEvent(xSelectionRequest.display, xSelectionRequest.requestor, False, NoEventMask, (XEvent *) &response);
            break;
        }
        default:break;
    }
}

long X11ClipboardAdapter::eventMask() {
    return 0;
}
