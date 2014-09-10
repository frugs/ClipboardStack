#include <iostream>
#include "PastePopListener.h"

using namespace std;

void PastePopListener::handleEvent(XEvent *event) {
    switch (event->type) {
        case KeyPress:
            cout << "Hotkey pressed! Woo!" << endl;
        default:
            break;
    }
}

long PastePopListener::eventMask() {
    return KeyPressMask;
}