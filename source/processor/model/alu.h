//===========================
//include guard
#ifndef ALU_H
#define ALU_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;

//===========================
//class declaration
class ALU {

  private:

    //forward components
    ReorderBuffer* const reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //instruction result
    int destinationRegister;
    int result;

    //position in the reorder buffer
    int nextReorderBufferIndex;
    int currentReorderBufferIndex;

  public:
    ALU(ReorderBuffer* const reorderBuffer);

    void execute();

    void pipe();

    void setOpcode(const int x);

    void setOperands(const int x[3]);

    void flush();

    void setReorderBufferIndex(const int i);
};

#endif