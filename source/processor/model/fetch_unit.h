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
    
    const Instructions instructions;
    int* const pc;
    DecodeIssueUnit* const decodeIssueUnit;
    const int issueWindowSize;
    Instruction* const currentInstructions;

    const int size;
    int head;
    int tail;
    int* const branchAddresses;

  //public functions
  public:

    FetchUnit(const Instructions instructions, int* const pc, DecodeIssueUnit* const decodeIssueUnit, 
      const int issueWindowSize);

    void execute();

    void print() const;

    void pipe();

    void flush();

    int getTail();

  //private functions
  private:

    void fetchInstructions(int num);

    bool isABranchInstruction(Instruction instruction) const;

    bool takeBranch(Instruction instruction);

    void storeBranchAddress();

    bool empty() const;

    void printBranchAddresses() const;

  //getter and setter functions
  public:
    
    Instruction getCurrentInstruction() const;

    void getCurrentInstructions(Instruction* const copy) const;
};

#endif