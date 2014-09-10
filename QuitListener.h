#pragma once
#include <X11/Xlib.h>
#include "IEventHandler.h"

class QuitListener : public IEventHandler {

public:
    void handleEvent(XEvent *event);

    long eventMask();

    bool shouldQuit();

private:
    bool _shouldQuit = false;

};