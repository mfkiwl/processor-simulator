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

  public:
    FetchUnit(const Instructions instructions, int* const pc, DecodeIssueUnit* const decodeIssueUnit, 
      const int issueWindowSize);

    void fetchInstructions(int num);

    void execute();

    void print() const;

    void pipe();

    void flush();

    //=============================
    // getter functions
    
    Instruction getCurrentInstruction() const;

    void getCurrentInstructions(Instruction* const copy) const;
};

#endif