#include <iostream>
#include <vector>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ClipboardStack.h"
#include "PastePopListener.h"
#include "QuitListener.h"

using namespace std;

int main() {
    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);

    ClipboardStack clipboardStack(display, root);
    clipboardStack.initialise();

    PastePopListener pastePopListener;

    QuitListener quitListener;

    vector<IEventHandler*> eventHandlers;
    eventHandlers.push_back(&clipboardStack);
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

    while (!quitListener.shouldQuit()) {
        XEvent event;
        XNextEvent(display, &event);

        for (auto eventHandler : eventHandlers) {
            eventHandler->handleEvent(&event);
        }
    }

    XUngrabKey(display, keyCode, keyModifiers, root);
    XCloseDisplay(display);
    return 0;
}