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

  //private members
  private:

    //forward components
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int nextOpcode;
    int currentOpcode;
    int nextOperands[3];
    int currentOperands[3];
    
    //position in the reorder buffer
    int nextReorderBufferIndex;
    int currentReorderBufferIndex;

    //instruction result
    int currentResult;

  //public functions
  public:

    ALU();

    ALU(ReorderBuffer* const reorderBuffer);

    void execute();

    void pipe();

    void flush();

  //getters and setters
  public:

    void setNextOpcode(const int x);

    void setNextOperands(const int x[3]);

    void setNextReorderBufferIndex(const int i);

    int getResult() const;

    int getReorderBufferIndex() const;

    void setReorderBufferPointer(ReorderBuffer* p);
};

#endif