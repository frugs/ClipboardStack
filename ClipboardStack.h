#include <stack>
#include <map>
#include <vector>

class ClipboardStack {

public:
    ClipboardStack();
    ~ClipboardStack();

    void addConversion(unsigned long type, std::vector<unsigned char> data);

    void push();

    void pop();

    //TODO: expose a slightly nicer interface
    std::map<unsigned long, std::vector<unsigned char>> * top();

private:
    std::stack<std::map<unsigned long, std::vector<unsigned char>>> _stack;

    bool isEmpty();

};