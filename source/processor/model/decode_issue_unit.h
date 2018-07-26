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

    //The number of instructions
    const int numInstructions;

    //Instruction given from the fetch unit
    Instruction* const nextInstructions;

    //the current instruction
    Instruction* const currentInstructions;

    bool* const currentInstructionsIssued;

    int reorderBufferIndex;

    //tells the processor whether or not to block the pipeline
    bool blockingFlag;

  //public functions
  public:
    DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, 
      ALUReservationStation* const aluReservationStation, 
      BranchUnitReservationStation* const branchUnitReservationStation, 
      LoadStoreUnitReservationStation* const loadStoreUnitReservationStation);

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

    bool getBlockingFlag() const;
};

#endif