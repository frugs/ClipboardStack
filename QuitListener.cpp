#include "QuitListener.h"

void QuitListener::handleEvent(XEvent event) {
    if (event.type == KeyPress) {
        _shouldQuit = true;
    }
}

long QuitListener::eventMask() {
    return KeyPressMask;
}
bool QuitListener::shouldQuit() {
    return _shouldQuit;
}