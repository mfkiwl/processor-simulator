//===========================
//include guard
#ifndef FETCH_UNIT_H
#define FETCH_UNIT_H

//===========================
//forward declared dependencies
class DecodeIssueUnit;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class FetchUnit {

  private:
    
    const Instructions instructionBuffer;
    int* const pc;
    DecodeIssueUnit* const decodeIssueUnit;
    const int issueWindowSize;
    Instruction* const instructions;

    int* const branchAddresses;

    const bool branchPrediction;

  //public functions
  public:

    FetchUnit(const Instructions instructionBuffer, int* const pc, DecodeIssueUnit* const decodeIssueUnit, 
      const int issueWindowSize, const bool branchPrediction);

    void execute();

    void print() const;

    void pipe();

    void flush();

  //private functions
  private:

    void printBranchAddresses();

    void fetchInstructions(int num);

    bool isABranchInstruction(Instruction instruction) const;

    bool takeBranch(Instruction instruction);

    int getBranchTargetAddress(Instruction instruction) const;

  //getter and setter functions
  public:

    void getInstructions(Instruction copy[]) const;
};

#endif