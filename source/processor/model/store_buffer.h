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

  //private member variables
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

  //public functions
  public:

    StoreBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps);

    void addToBuffer(const int address, const int value, const int reorderBufferIndex);

    void incrementSteps();

    void writeIfReady();

    void flush();

    void print() const;
};

#endif