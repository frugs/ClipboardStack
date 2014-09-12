#include <stack>
#include <map>

class ClipboardStack {

public:
    ClipboardStack();
    ~ClipboardStack();

    void addConversion(unsigned long type, unsigned char *data);

    void push();

    void pop();

    std::map<unsigned long, unsigned char *> * top();

private:
    std::stack<std::map<unsigned long, unsigned char*>> _stack;

    bool isEmpty();

};