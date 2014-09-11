class ClipboardStack {

public:
    class ArbitraryData {
    public:
        ArbitraryData(char *start, unsigned long sizeInBytes) {
            _start = start;
            _sizeInBytes = sizeInBytes;
        };

        ~ArbitraryData() {
            delete[] _start;
        }

    private:
        char *_start;
        unsigned long _sizeInBytes;

    };

    void addConversion(ArbitraryData type, ArbitraryData data);

    void push();

    void pop();

};