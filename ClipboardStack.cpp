#include "ClipboardStack.h"

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
        default:
            break;
    }
}

long ClipboardStack::eventMask() {
    return 0;
}
