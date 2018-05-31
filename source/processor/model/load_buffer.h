//===========================
//include guard
#ifndef LOAD_BUFFER_H
#define LOAD_BUFFER_H

//===========================
//forward declared dependencies
class Memory;
class ReorderBuffer;

//===========================
//class declaration
class LoadBuffer {

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
    const int DESTINATION;
    const int ADDRESS;
    const int REORDER_BUFFER_INDEX;
    const int STEP;

    //the number of entries in the buffer ready to read
    int numberReady;

  public:
	  LoadBuffer(Memory* memory, ReorderBuffer* reorderBuffer, int size, int steps);

    void flush();

    void stepInstructions();

    void checkIfReady();

    //return 1 if we are waiting for a write operation to complete
    int waitingForReadOperation();

    void addToBuffer(int destinationRegister, int address, int reorderBufferIndex);

    void readIfReady();
};

#endif