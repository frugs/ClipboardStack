#include <iostream>
#include <X11/Xlib.h>
#include "X11ClipboardAdapter.h"

struct X11ClipboardAdapter::Property {
    Atom type;
    int format;
    unsigned long itemCount;
    unsigned char *data;
};

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


    Property result = { actualType, actualFormat, itemCount, ret };
    return result;
}

X11ClipboardAdapter::X11ClipboardAdapter(Display *display, Window window, ClipboardStack *clipboardStack) {
    _display = display;
    _window = window;
    _clipboardStack = clipboardStack;

    _XA_CLIPBOARD = XInternAtom(_display, "CLIPBOARD", 0);
    _XA_TARGETS = XInternAtom(_display, "TARGETS", True);
}

void X11ClipboardAdapter::initialise() {
    Window owner = XGetSelectionOwner(_display, _XA_CLIPBOARD);

    if (owner == None) {
        XSetSelectionOwner(_display, _XA_CLIPBOARD, _window, CurrentTime);
    } else {
        onCopy();
    }
}

void X11ClipboardAdapter::onCopy() {
    XConvertSelection(_display, _XA_CLIPBOARD, _XA_TARGETS, _XA_CLIPBOARD, _window, CurrentTime);
}

void X11ClipboardAdapter::handleEvent(XEvent *event) {
    switch (event->type) {
        case SelectionNotify: {
            XSelectionEvent *xSelection = &event->xselection;
            Property prop = readProperty(_XA_CLIPBOARD);

            if (xSelection->target == _XA_TARGETS) {
                std::cout << "SelectionNotify->XA_TARGETS" << std::endl;

                for (int i = 0; i < prop.itemCount; i++) {
                    XConvertSelection(_display, _XA_CLIPBOARD, ((Atom *) prop.data)[i], _XA_CLIPBOARD, _window, xSelection->time);
                }
            } else {
                std::cout << "SelectionNotify->" << XGetAtomName(_display, xSelection->target) << std::endl;
            }
            break;
        }
        case SelectionClear:
            std::cout << "SelectionClear" << std::endl;
            break;
        default:break;
    }
}

long X11ClipboardAdapter::eventMask() {
    return 0;
}
