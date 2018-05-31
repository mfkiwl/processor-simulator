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

    //forward components
    Memory* memory;
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    int bufferSize;
    int writeCycles;
    int readCycles;

    //Read and write buffers to store load and store instruciton in operation
    StoreBuffer storeBuffer;
    LoadBuffer loadBuffer;

    //position in the reorder buffer
    int reorderBufferIndex;

  public:
    LoadStoreUnit(Memory* memory, ReorderBuffer* reorderBuffer);

    void execute();

    void pipe();

    void setOpcode(int x);

    void setOperands(int x[3]);

    void flush();

    void setReorderBufferIndex(int i);

    int waitingForStore();
};

#endif