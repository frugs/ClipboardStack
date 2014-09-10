#pragma once
#include <X11/Xlib.h>

class IEventHandler {

public:
    virtual void handleEvent(XEvent *event) = 0;
    virtual long eventMask() = 0;

protected:
    ~IEventHandler() {}

};