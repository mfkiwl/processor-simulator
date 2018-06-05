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
    Instruction currentInstruction;

  public:
    FetchUnit(const Instructions instructions, int* const pc, DecodeIssueUnit* const decodeIssueUnit);

    void execute();

    void print() const;

    void pipe();

    void flush();

    //=============================
    // getter functions
    
    Instruction getCurrentInstruction() const;
};

#endif