//===========================
//include guard
#ifndef BRANCH_UNIT_RESERVATION_STATION_H
#define BRANCH_UNIT_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class BranchUnit;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class BranchUnitReservationStation {

  //private member variables
  private:
    
    RegisterFile* const registerFile;
    BranchUnit* const branchUnit;

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
    
    BranchUnitReservationStation(RegisterFile* const registerFile, BranchUnit* const branchUnit, const int size, 
      const int issueWindowSize);

    void execute();

    bool freeSpace() const;

    void pipe();

    void flush();

    void print() const;

  //private functions
  private:

    int findFreePosition() const;

    void addInstruction(const Instruction instruction, const int rbi);

    bool readyToDispatch(const int index) const;

    //dispatch bound fetch
    void fetchOperands(const int index);

  //getters and setters
  public:

    void getCurrentInstructions(Instruction* const copy) const;

    void getCurrentReorderBufferIndexes(int* const copy) const;

    void setNextInstruction(const Instruction instruction);

    void setNextReorderBufferIndex(const int index);

};

#endif