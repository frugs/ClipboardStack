#pragma once
class X11InternalAtoms {

public:
    X11InternalAtoms();
    X11InternalAtoms(Display *display);

    Atom getXATargetsAtom();
    Atom getXAClipboardAtom();

private:
    Atom _XA_TARGETS;
    Atom _XA_CLIPBOARD;

};