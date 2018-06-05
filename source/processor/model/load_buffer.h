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

  private:

    Memory* const memory;
    ReorderBuffer* const reorderBuffer;

    //write buffer to hold inflight write operation info
    int** buffer;
    const int size;
    int head;
    int tail;
    const int steps;

    //index constants
    static const int entryFields = 4;
    static const int DESTINATION = 0;
    static const int ADDRESS = 1;
    static const int REORDER_BUFFER_INDEX = 2;
    static const int STEP = 3;

  public:
	  LoadBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps);

    void flush();

    void stepInstructions();

    //return 1 if we are waiting for a write operation to complete
    int waitingForReadOperation() const;

    void addToBuffer(const int destinationRegister, const int address, const int reorderBufferIndex);

    void readIfReady();
};

#endif