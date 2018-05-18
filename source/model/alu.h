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

    //forward components
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;

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
    ALU(RegisterFile* registerFile, ReorderBuffer* reorderBuffer);

    void execute();

    void pipe();

    void setOpcode(int x);

    void setOperands(int x[3]);

    void flush();

    void setReorderBufferIndex(int i);
};

#endif