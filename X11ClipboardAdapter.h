#include <X11/Xlib.h>
#include "IEventHandler.h"
#include "ClipboardStack.h"

class X11ClipboardAdapter : public IEventHandler {

public:
    X11ClipboardAdapter(Display *display, Window window, ClipboardStack *clipboardStack);

    void initialise();

    void handleEvent(XEvent *event);

    long eventMask();

    void onPaste();

    void onPastePop();

private:

    struct Property {
        Atom type;
        int format;
        unsigned long itemCount;
        unsigned char *data;
    };

    Atom _XA_CLIPBOARD;
    Atom _XA_TARGETS;

    Display *_display;
    Window _window;
    ClipboardStack *_clipboardStack;

    unsigned long _targetCount;

    X11ClipboardAdapter::Property readProperty(Atom property);

    void onCopy();
};