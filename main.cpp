#include <iostream>
#include <vector>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "X11ClipboardAdapter.h"
#include "PastePopListener.h"
#include "QuitListener.h"

using namespace std;

int main() {
    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);
    Window window = XCreateSimpleWindow(display, root, 0, 0, 100, 100, 0, 0, 0);

    X11InternalAtoms x11InternalAtoms(display);

    ClipboardStack clipboardStack;

    X11ClipboardAdapter x11ClipboardAdapter(display, window, x11InternalAtoms, &clipboardStack);
    x11ClipboardAdapter.initialise();

    PastePopListener pastePopListener(display, root, x11InternalAtoms, &clipboardStack);

    QuitListener quitListener;

    vector<IEventHandler*> eventHandlers;
    eventHandlers.push_back(&x11ClipboardAdapter);
    eventHandlers.push_back(&pastePopListener);
    eventHandlers.push_back(&quitListener);

    long eventMask = 0;
    for (auto eventHandler : eventHandlers) {
        eventMask = eventMask | eventHandler->eventMask();
    }

    unsigned int keyModifiers = Mod1Mask | Mod4Mask;
    int keyCode = XKeysymToKeycode(display, XK_V);

    XGrabKey(display, keyCode, keyModifiers, root, False, GrabModeAsync, GrabModeAsync);

    XSelectInput(display, root, eventMask);
    XSelectInput(display, window, eventMask);

    while (!quitListener.shouldQuit()) {
        XEvent event;
        XNextEvent(display, &event);

        for (auto eventHandler : eventHandlers) {
            eventHandler->handleEvent(event);
        }
    }

    XUngrabKey(display, keyCode, keyModifiers, root);
    XCloseDisplay(display);
    return 0;
}