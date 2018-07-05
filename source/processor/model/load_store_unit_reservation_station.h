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

    bool usingLoadQueue;

  private:

    bool loadQueueReadyToDispatch(const int index) const;

    bool storeQueueReadyToDispatch(const int index) const;

    //dispatch bound fetch
    void loadQueueFetchOperands(const int index);

    void storeQueueFetchOperands(const int index);

    void incrementAges();

  public:
    LoadStoreUnitReservationStation(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, LoadStoreUnit* const loadStoreUnit, const int size);

    void execute();

    void loadQueueAddInstruction(const Instruction instruction, const int rbi);

    void storeQueueAddInstruction(const Instruction instruction, const int rbi);

    bool spaceInLoadQueue() const;

    bool spaceInStoreQueue() const;

    bool loadQueueEmpty() const;

    bool storeQueueEmpty() const;

    void chooseNextQueueToDispatch();

    void pipe();

    void flush();

    void print() const;

    void getLoadQueueInstructions(Instruction* const copy) const;

    void getLoadQueueReorderBufferIndexes(int* const copy) const;

    void getLoadQueueAges(int* const copy) const;

    void getStoreQueueInstructions(Instruction* const copy) const;

    void getStoreQueueReorderBufferIndexes(int* const copy) const;

    void getStoreQueueAges(int* const copy) const;

    void setLoadQueueNextInstruction(const Instruction instruction);

    void setLoadQueueNextReorderBufferIndex(const int index);

    void setStoreQueueNextInstruction(const Instruction instruction);

    void setStoreQueueNextReorderBufferIndex(const int index);

};

#endif