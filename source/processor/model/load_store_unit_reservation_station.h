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

  //private member variables
  private:

    RegisterFile* const registerFile;
    ReorderBuffer* const reorderBuffer;
    LoadStoreUnit* const loadStoreUnit;

    int tail;
    int head;
    const int size;
    Instruction* const instructions;
    int* const reorderBufferIndexes;

    const int issueWindowSize;

    Instruction* const nextInstructions;
    int nextReorderBufferIndex;

    int opcode;
    int operands[3];
    int reorderBufferIndex;

    int dispatchIndex;

  //public functions
  public:

    LoadStoreUnitReservationStation(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, 
      LoadStoreUnit* const loadStoreUnit, const int size, const int issueWindowSize);

    void execute();

    void addInstruction(const Instruction instruction, const int rbi);

    bool freeSpace() const;

    void pipe();

    void flush();

    void print() const;

  //private functions
  private:

    bool readyToDispatch(const int index) const;

    void fetchOperands(const int index);

  //getters and setters
  public:

    void getCurrentInstructions(Instruction* const copy) const;

    void getCurrentReorderBufferIndexes(int* const copy) const;

    void setNextInstruction(const Instruction instruction);

    void setNextReorderBufferIndex(const int index);

};

#endif