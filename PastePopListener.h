#include <X11/Xlib.h>
#include "IEventHandler.h"
#include "ClipboardStack.h"
#include "X11InternalAtoms.h"

class PastePopListener : public IEventHandler {

public:
    PastePopListener(Display *display, Window rootWindow, X11InternalAtoms x11InternalAtoms, ClipboardStack *clipboardStack);

    virtual void handleEvent(XEvent event);

    virtual long eventMask();

private:
    Display *_display;

    Window _rootWindow;

    ClipboardStack *_clipboardStack;

    X11InternalAtoms _x11InternalAtoms;

    bool _pop = false;

    XKeyEvent createPasteKeyEvent(Window &win, bool press);
};
