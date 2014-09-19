#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "PastePopListener.h"

using namespace std;

PastePopListener::PastePopListener(Display *display, Window rootWindow, X11InternalAtoms x11InternalAtoms, ClipboardStack *clipboardStack) {
    _display = display;
    _rootWindow = rootWindow;
    _clipboardStack = clipboardStack;
    _x11InternalAtoms = x11InternalAtoms;
}

void PastePopListener::handleEvent(XEvent event) {
    if (event.type == KeyPress) {
        Window focusWindow;
        int mode;

        XGetInputFocus(_display, &focusWindow, &mode);

        XKeyEvent pressPaste = createPasteKeyEvent(focusWindow, true);
        XKeyEvent releasePaste = createPasteKeyEvent(focusWindow, false);

        XSendEvent(_display, focusWindow, True, KeyPressMask, (XEvent*) &pressPaste);
        XSendEvent(_display, focusWindow, True, KeyPressMask, (XEvent*) &releasePaste);

        _pop = true;
        cout << "Hotkey pressed! Woo!" << endl;
    } else if (event.type == SelectionRequest
            && event.xselectionrequest.target != _x11InternalAtoms.getXATargetsAtom()
            && _pop) {
        _clipboardStack->pop();
        _pop = false;
    }
}

long PastePopListener::eventMask() {
    return KeyPressMask;
}

XKeyEvent PastePopListener::createPasteKeyEvent(Window &win, bool press)
{
    XKeyEvent event;

    event.display     = _display;
    event.window      = win;
    event.root        = _rootWindow;
    event.subwindow   = None;
    event.time        = CurrentTime;
    event.x           = 1;
    event.y           = 1;
    event.x_root      = 1;
    event.y_root      = 1;
    event.same_screen = True;
    event.keycode     = XKeysymToKeycode(_display, XK_V);
    event.state       = ControlMask;

    if(press)
        event.type = KeyPress;
    else
        event.type = KeyRelease;

    return event;
}