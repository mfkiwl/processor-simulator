//===========================
//include guard
#ifndef STORE_BUFFER_H
#define STORE_BUFFER_H

//===========================
//forward declared dependencies
class Memory;
class ReorderBuffer;

//===========================
//class declaration
class StoreBuffer {

  private:

    Memory* const memory;
    ReorderBuffer* const reorderBuffer;

    //write buffer to hold inflight write operation info
    const int size;
    int** const buffer;
    int head;
    int tail;
    const int steps;

    //index constants
    static const int entryFields = 4;
    static const int ADDRESS = 0;
    static const int VALUE = 1;
    static const int REORDER_BUFFER_INDEX = 2;
    static const int STEP = 3;

  public:
    StoreBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps);

    void flush();

    void addToBuffer(const int address, const int value, const int reorderBufferIndex);

    void stepInstructions();

    //return 1 if we are waiting for a write operation to complete
    int waitingForStore() const;

    void writeIfReady();
};

#endif