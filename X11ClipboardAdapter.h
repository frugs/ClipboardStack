#include <X11/Xlib.h>
#include "IEventHandler.h"
#include "ClipboardStack.h"
#include "X11InternalAtoms.h"

class X11ClipboardAdapter : public IEventHandler {

public:
    X11ClipboardAdapter(Display *display, Window window, X11InternalAtoms _x11InternalAtoms, ClipboardStack *clipboardStack);

    void initialise();

    void handleEvent(XEvent event);

    long eventMask();

private:
    struct Property {
        int format;
        unsigned long itemCount;
        unsigned char *data;
    };

    Display *_display;
    Window _window;

    X11InternalAtoms _x11InternalAtoms;

    ClipboardStack *_clipboardStack;

    unsigned long _targetCount;

    X11ClipboardAdapter::Property readProperty(Atom property);

    void onCopy();
};