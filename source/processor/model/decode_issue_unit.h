//===========================
//include guard
#ifndef DECODE_ISSUE_UNIT_H
#define DECODE_ISSUE_UNIT_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;
class ALUReservationStation;
class BranchUnitReservationStation;
class LoadStoreUnitReservationStation;
class LoadStoreUnit;
class LoadQueue;
class StoreQueue;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class DecodeIssueUnit {

  //private member variables
  private:
    
    //forward components
    RegisterFile* const registerFile;
    ReorderBuffer* const reorderBuffer;
    ALUReservationStation* const aluReservationStation;
    BranchUnitReservationStation* const branchUnitReservationStation;
    StoreQueue* const storeQueue;
    LoadQueue* const loadQueue;

    const int issueWindowSize;

    //Instruction given from the fetch unit
    Instruction* const nextInstructions;
    int* const nextBranchAddresses;

    Instruction* const instructions;
    int* const branchAddresses;
    bool* const instructionsIssued;
    int* const reorderBufferIndexes;

    const bool branchPrediction;

  //public functions
  public:
    DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, 
      ALUReservationStation* const aluReservationStation, 
      BranchUnitReservationStation* const branchUnitReservationStation, 
      StoreQueue* const storeQueue, LoadQueue* const loadQueue, const int issueWindowSize, 
      const bool branchPrediction);

    void execute();

    void issue(int instructionToIssue);

    void moveInstructions();

    void pipe();

    void pipeInstruction(int instructionToIssue);

    void flush();

    void print() const;

    bool allInstructionsIssued() const;

    int numFreeSpaces() const;

  //getters and setters
  public:

    void setNextInstructions(const Instruction* const x);

    void getInstructions(Instruction* const copy) const;

    void getReorderBufferIndexes(int* const copy) const;

    void setNextBranchAddresses(const int* const x);
};

#endif