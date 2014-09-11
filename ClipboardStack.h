class ClipboardStack {

public:
    struct Type { void *id; };

    void addConversion(Type type, void *data);

    void push();

    void pop();

};