#include <X11/Xlib.h>
#include "IEventHandler.h"

class ClipboardStack : public IEventHandler {

public:
    ClipboardStack(Display *display, Window window);

    void initialise();

    void handleEvent(XEvent *event);

    long eventMask();

    void onCopy();

    void onPaste();

    void onPastePop();

private:
    Atom _XA_CLIPBOARD;
    Atom _XA_TARGETS;

    Display *_display;
    Window _window;
};