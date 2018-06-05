//===========================
//include guard
#ifndef LOAD_STORE_UNIT_H
#define LOAD_STORE_UNIT_H

//===========================
//forward declared dependencies
class Memory;
class ReorderBuffer;

//=================================
// included dependencies
#include "store_buffer.h"
#include "load_buffer.h"

//===========================
//class declaration
class LoadStoreUnit {

  private:

    //forward components
    Memory* const memory;
    ReorderBuffer* const reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    const int bufferSize;
    const int writeCycles;
    const int readCycles;

    //Read and write buffers to store load and store instruciton in operation
    StoreBuffer storeBuffer;
    LoadBuffer loadBuffer;

    //position in the reorder buffer
    int reorderBufferIndex;

  public:
    LoadStoreUnit(Memory* const memory, ReorderBuffer* const reorderBuffer);

    void execute();

    void pipe();

    void setOpcode(const int x);

    void setOperands(const int x[3]);

    void flush();

    void setReorderBufferIndex(const int i);

    int waitingForStore() const ;
};

#endif