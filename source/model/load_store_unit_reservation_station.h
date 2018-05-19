//===========================
//include guard
#ifndef LOAD_STORE_UNIT_RESERVATION_STATION_H
#define LOAD_STORE_UNIT_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;
class LoadStoreUnit;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class LoadStoreUnitReservationStation {

    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;
    LoadStoreUnit* loadStoreUnit;

    int tail;
    int head;
    int size;
    Instruction* instructions;
    int* reorderBufferIndexes;

    int opcode;
    int* operands;
    int reorderBufferIndex;

  public:
    LoadStoreUnitReservationStation(RegisterFile* registerFile, ReorderBuffer* reorderBuffer, LoadStoreUnit* loadStoreUnit);

    void execute();

    void addInstruction(Instruction instruction, int rbi);

    int spaceInQueue();

    void pipe();

    void flush();

    void print();

  private:

    int readyToDispatch(int index);

    //dispatch bound fetch
    void dispatch(int index);

};

#endif