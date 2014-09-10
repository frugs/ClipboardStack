#include <iostream>
#include <X11/Xlib.h>
#include "ClipboardStack.h"

struct ClipboardStack::Property {
    Atom type;
    int format;
    unsigned long itemCount;
    unsigned char *data;
};

ClipboardStack::Property ClipboardStack::readProperty(Atom property) {
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

ClipboardStack::ClipboardStack(Display *display, Window window) {
    _display = display;
    _window = window;

    _XA_CLIPBOARD = XInternAtom(_display, "CLIPBOARD", 0);
    _XA_TARGETS = XInternAtom(_display, "TARGETS", True);
}

void ClipboardStack::initialise() {
    Window owner = XGetSelectionOwner(_display, _XA_CLIPBOARD);

    if (owner == None) {
        XSetSelectionOwner(_display, _XA_CLIPBOARD, _window, CurrentTime);
    } else {
        onCopy();
    }
}

void ClipboardStack::onCopy() {
    XConvertSelection(_display, _XA_CLIPBOARD, _XA_TARGETS, _XA_CLIPBOARD, _window, CurrentTime);
}

void ClipboardStack::handleEvent(XEvent *event) {
    switch (event->type) {
        case SelectionNotify:
            std::cout << "SelectionNotify" << std::endl;

            if (event->xselection.target == _XA_TARGETS) {
                Property prop = readProperty(_XA_CLIPBOARD);
            }

            break;
        case SelectionClear:
            std::cout << "SelectionClear" << std::endl;
            break;
    }
}

long ClipboardStack::eventMask() {
    return 0;
}
