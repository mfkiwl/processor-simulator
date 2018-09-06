//===========================
//include guard
#ifndef ALU_H
#define ALU_H

//===========================
//forward declared dependencies
class ReorderBuffer;

//===========================================
// included dependencies
#include "constants.h"

//===========================
//class declaration
class ALU {

  //private member variables
  private:

    //forward components
    ReorderBuffer* reorderBuffer;
    
    //next information
    int nextOpcode;
    int nextOperands[numOfOperands];
    int nextReorderBufferIndex;

    //current information
    int opcode;
    int operands[numOfOperands];
    int reorderBufferIndex;

    //instruction result
    int result;

  //public functions
  public:

    ALU();

    void execute();

    void pipe();

    void flush();

  //getters and setters
  public:

    void setNextOpcode(const int x);

    void setNextOperands(const int x[numOfOperands]);

    void setNextReorderBufferIndex(const int i);

    int getResult() const;

    int getReorderBufferIndex() const;

    void setReorderBufferPointer(ReorderBuffer* p);
};

#endif