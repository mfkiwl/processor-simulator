//===========================
//include guard
#ifndef LOAD_STORE_UNIT_H
#define LOAD_STORE_UNIT_H

//===========================
//forward declared dependencies
class Memory;
class ReorderBuffer;
class ALUReservationStation;

//=================================
// included dependencies
#include "store_buffer.h"
#include "load_buffer.h"

//===========================
//class declaration
class LoadStoreUnit {

  //private member variables
  private:

    //forward components
    Memory* const memory;
    ReorderBuffer* const reorderBuffer;

    //decoded instruction
    int nextOpcode;
    int nextOperands[3];
    int nextReorderBufferIndex;

    int opcode;
    int operands[3];
    int reorderBufferIndex;

    const int bufferSize;
    const int writeCycles;
    const int readCycles;

    //Read and write buffers to store load and store instruciton in operation
    StoreBuffer storeBuffer;
    LoadBuffer loadBuffer;

    ALUReservationStation* const aluReservationStation;

  //public functions
  public:

    LoadStoreUnit(Memory* const memory, ReorderBuffer* const reorderBuffer, 
      ALUReservationStation* const aluReservationStation);

    void execute();

    void pipe();

    void flush();

  //getters and setters
  public:

    void setNextOpcode(const int x);

    void setNextOperands(const int x[3]);

    void setNextReorderBufferIndex(const int i);
};

#endif