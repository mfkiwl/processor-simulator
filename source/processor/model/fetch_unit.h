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
    
    Instructions instructions;
    int* pc;
    DecodeIssueUnit* decodeIssueUnit;
    Instruction currentInstruction;

  public:
    FetchUnit(Instructions instructions, int* pc, DecodeIssueUnit* decodeIssueUnit);

    void execute();

    void print();

    void pipe();

    void flush();
};

#endif