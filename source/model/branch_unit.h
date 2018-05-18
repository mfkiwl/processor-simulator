//===========================
//include guard
#ifndef BRANCH_UNIT_H
#define BRANCH_UNIT_H

//===========================
//forward declared dependencies
class ReorderBuffer;

//=================================
// included dependencies
#include "constants.h"

//===========================
//class declaration
class BranchUnit {
	
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //position in the reorder buffer
    int reorderBufferIndex;

    //if the branch condition is met or not
    int successful;

  public:
    BranchUnit(ReorderBuffer* reorderBuffer);

    void execute();

    void pipe();

    void setOpcode(int x);

    void setOperands(int x[3]);

    void flush();

    void setReorderBufferIndex(int i);
};

#endif