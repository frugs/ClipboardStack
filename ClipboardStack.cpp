#include <cstring>
#include <iostream>
#include "ClipboardStack.h"

ClipboardStack::ClipboardStack() {
    push();
}

ClipboardStack::~ClipboardStack() {
    while (!isEmpty()) {
        pop();
    }
}

void ClipboardStack::addConversion(unsigned long type, std::vector<unsigned char> data) {
    _stack.top()[type] = data;
}

void ClipboardStack::push() {
    _stack.emplace();
}

void ClipboardStack::pop() {
    if (_stack.size() > 1) {
        for (auto &entry : *top()) {
            entry.second.clear();
        }
        _stack.pop();
    }
}

//TODO: Replace this with a nicer API
std::map<unsigned long, std::vector<unsigned char>>* ClipboardStack::top() {
    return &_stack.top();
}

bool ClipboardStack::isEmpty() {
    return _stack.empty();
}
