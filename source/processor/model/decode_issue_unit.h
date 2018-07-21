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

  private:
    
    //forward components
    RegisterFile* const registerFile;
    ReorderBuffer* const reorderBuffer;
    ALUReservationStation* const aluReservationStation;
    BranchUnitReservationStation* const branchUnitReservationStation;
    LoadStoreUnitReservationStation* const loadStoreUnitReservationStation;

    //Instruction given from the fetch unit
    Instruction nextInstruction;

    //the current instruction
    Instruction currentInstruction;

    int reorderBufferIndex;

    //tells the processor whether or not to block the pipeline
    bool blockingFlag;

  public:
    DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, ALUReservationStation* const aluReservationStation, BranchUnitReservationStation* const branchUnitReservationStation, LoadStoreUnitReservationStation* const loadStoreUnitReservationStation);

    void execute();

    void registerRename();

    void issue();

    void pipe();

    void print() const;

    void setNextInstruction(const Instruction x);

    void flush();

    Instruction getCurrentInstruction() const;

    bool getBlockingFlag() const;
};

#endif