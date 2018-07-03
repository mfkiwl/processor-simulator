//===========================
//include guard
#ifndef LOAD_STORE_UNIT_RESERVATION_STATION_H
#define LOAD_STORE_UNIT_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;
class LoadStoreUnit;
class Instruction;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class LoadStoreUnitReservationStation {

    RegisterFile* const registerFile;
    ReorderBuffer* const reorderBuffer;
    LoadStoreUnit* const loadStoreUnit;

    Instruction loadQueueNextInstruction;
    int loadQueueNextReorderBufferIndex;

    int loadQueueTail;
    int loadQueueHead;
    const int loadQueueSize;
    Instruction* const loadQueueInstructions;
    int* loadQueueAges;
    int* const loadQueueReorderBufferIndexes;

    Instruction storeQueueNextInstruction;
    int storeQueueNextReorderBufferIndex;

    int storeQueueTail;
    int storeQueueHead;
    const int storeQueueSize;
    Instruction* const storeQueueInstructions;
    int* storeQueueAges;
    int* const storeQueueReorderBufferIndexes;

    int opcode;
    int operands[3];
    int reorderBufferIndex;

    int dispatchIndex;

  private:

    bool readyToDispatch(const int index) const;

    //dispatch bound fetch
    void fetchOperands(const int index);

    void incrementAges();

  public:
    LoadStoreUnitReservationStation(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, LoadStoreUnit* const loadStoreUnit, const int size);

    void execute();

    void addInstruction(const Instruction instruction, const int rbi);

    bool spaceInQueue() const;

    void pipe();

    void flush();

    void print() const;

    void getLoadQueueInstructions(Instruction* const copy) const;

    void getLoadQueueReorderBufferIndexes(int* const copy) const;

    void getLoadQueueAges(int* const copy) const;

    void getStoreQueueInstruction(Instruction* const copy) const;

    void getStoreQueueReorderBufferIndexes(int* const copy) const;

    void getStoreQueueAges(int* const copy) const;

    void setLoadQueueNextInstruction(const Instruction instruction);

    void setLoadQueueNextReorderBufferIndex(const int index);

    void setStoreQueueNextInstruction(const Instruction instruction);

    void setStoreQueueNextReorderBufferIndex(const int index);

};

#endif