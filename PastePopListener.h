#include <X11/Xlib.h>
#include "IEventHandler.h"

class PastePopListener : public IEventHandler {

public:
    virtual void handleEvent(XEvent *event);

    virtual long eventMask();

};
