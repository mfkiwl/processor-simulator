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
    LoadStoreUnitReservationStation* const loadStoreUnitReservationStation;

    const int issueWindowSize;

    //Instruction given from the fetch unit
    Instruction* const nextInstructions;

    //the current instruction
    Instruction* const currentInstructions;

    bool* const currentInstructionsIssued;

    int reorderBufferIndex;

  //public functions
  public:
    DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, 
      ALUReservationStation* const aluReservationStation, 
      BranchUnitReservationStation* const branchUnitReservationStation, 
      LoadStoreUnitReservationStation* const loadStoreUnitReservationStation, const int issueWindowSize);

    void execute();

    void issue(int instructionToIssue);

    void pipe();

    void flush();

    void print() const;

    bool allInstructionsIssued() const;

  //getters and setters
  public:

    void setNextInstructions(const Instruction* const x);

    Instruction getCurrentInstruction() const;

    void getCurrentInstructions(Instruction* const copy) const;
};

#endif