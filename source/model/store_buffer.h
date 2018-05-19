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

    Memory* memory;
    ReorderBuffer* reorderBuffer;

    //write buffer to hold inflight write operation info
    int** buffer;
    int size;
    int head;
    int tail;
    int steps;

    int entryFields;

    //index constants
    const int ADDRESS;
    const int VALUE;
    const int REORDER_BUFFER_INDEX;
    const int STEP;

    int numberReady;

  public:
    StoreBuffer(Memory* memory, ReorderBuffer* reorderBuffer, int size, int steps);

    void flush();

    void addToBuffer(int address, int value, int reorderBufferIndex);

    void stepInstructions();

    void checkIfReady();

    //return 1 if we are waiting for a write operation to complete
    int waitingForStore();

    void writeIfReady();
};

#endif