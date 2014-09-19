#include <X11/Xlib.h>
#include "X11InternalAtoms.h"

X11InternalAtoms::X11InternalAtoms(Display *display) {
    _XA_CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0);
    _XA_TARGETS = XInternAtom(display, "TARGETS", True);
}

X11InternalAtoms::X11InternalAtoms() {}

Atom X11InternalAtoms::getXATargetsAtom() {
    return _XA_TARGETS;
}

Atom X11InternalAtoms::getXAClipboardAtom() {
    return _XA_CLIPBOARD;
}
