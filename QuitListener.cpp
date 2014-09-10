#include "QuitListener.h"

void QuitListener::handleEvent(XEvent *event) {
    switch (event->type) {
        case KeyPress:
            _shouldQuit = true;
        default:
            break;
    }
}

long QuitListener::eventMask() {
    return KeyPressMask;
}
bool QuitListener::shouldQuit() {
    return _shouldQuit;
}