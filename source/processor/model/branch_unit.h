//===========================
//include guard
#ifndef BRANCH_UNIT_H
#define BRANCH_UNIT_H

//===========================
//forward declared dependencies
class ReorderBuffer;

//===========================
//class declaration
class BranchUnit {

  private:
	
    ReorderBuffer* const reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //position in the reorder buffer
    int reorderBufferIndex;

    //if the branch condition is met or not
    int successful;

  public:
    BranchUnit(ReorderBuffer* const reorderBuffer);

    void execute();

    void pipe();

    void setOpcode(const int x);

    void setOperands(const int x[3]);

    void flush();

    void setReorderBufferIndex(const int i);
};

#endif