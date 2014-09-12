#include "ClipboardStack.h"

ClipboardStack::ClipboardStack() {
    push();
}

ClipboardStack::~ClipboardStack() {
    while (!isEmpty()) {
        pop();
    }
}

void ClipboardStack::addConversion(unsigned long type, unsigned char *data) {
    delete _stack.top()[type];
    _stack.top()[type] = new unsigned char(*data);
}

void ClipboardStack::push() {
    _stack.emplace();
}

void ClipboardStack::pop() {
    for (auto &entry : *top()) {
        delete entry.second;
    }
    _stack.pop();
}

std::map<unsigned long, unsigned char *>* ClipboardStack::top() {
    return &_stack.top();
}

bool ClipboardStack::isEmpty() {
    return _stack.empty();
}
