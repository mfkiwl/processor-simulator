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
    int nextOpcode;
    int currentOpcode;
    int nextOperands[3];
    int currentOperands[3];

    //position in the reorder buffer
    int nextReorderBufferIndex;
    int currentReorderBufferIndex;

    //if the branch condition is met or not
    bool successful;

  public:
    BranchUnit(ReorderBuffer* const reorderBuffer);

    void execute();

    void pipe();

    void setNextOpcode(const int x);

    void setNextOperands(const int x[3]);

    void flush();

    void setNextReorderBufferIndex(const int i);

    bool getSuccessful() const;

    int getReorderBufferIndex() const;
};

#endif