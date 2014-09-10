#include <iostream>
#include "PastePopListener.h"

using namespace std;

void PastePopListener::handleEvent(XEvent *event) {
    if (event->type == KeyPress) {
        cout << "Hotkey pressed! Woo!" << endl;
    }
}

long PastePopListener::eventMask() {
    return KeyPressMask;
}