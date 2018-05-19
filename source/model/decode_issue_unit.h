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
    
    //forward components
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;
    ALUReservationStation* aluReservationStation;
    BranchUnitReservationStation* branchUnitReservationStation;
    LoadStoreUnitReservationStation* loadStoreUnitReservationStation;
    LoadStoreUnit* loadStoreUnit;

    //Instruction given from the fetch unit
    Instruction nextInstruction;

    //the current instruction
    Instruction currentInstruction;

    int reorderBufferIndex;

    //tells the processor whether or not to block the pipeline
    int* blockingFlag;

  public:
    DecodeIssueUnit(RegisterFile* registerFile, ReorderBuffer* reorderBuffer, ALUReservationStation* aluReservationStation, BranchUnitReservationStation* branchUnitReservationStation, LoadStoreUnitReservationStation* loadStoreUnitReservationStation, int* blockingFlag);

    void execute();

    void issue();

    void pipe();

    void print();

    void setNextInstruction(Instruction x);

    void flush();
};

#endif