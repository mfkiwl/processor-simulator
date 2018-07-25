//===========================
//include guard
#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

//===========================
//forward declared dependencies
class RegisterFile;
class Memory;
class Instruction;

//=================================
// included dependencies
#include "constants.h"

//===========================
//class declaration
class ReorderBuffer {

  //private member variables
  private:

    RegisterFile* const registerFile;
    Memory* const memory;

    int* const pc;
    bool flushFlag;
    int* const runningFlag;

    const int bufferSize;
    int** const buffer;
    int head;
    int tail;

    const int numFields;

    //stores the original instruction issued
    Instruction* const instructions;

    //no of instruction executed by the processor
    int* const noOfInstructionsExecuted;

  //public functions
  public:

    ReorderBuffer(RegisterFile* const registerFile, Memory* const memory, int* const pc, int* const runningFlag, 
    int* const noOfInstructionsExecuted, const int bufferSize, const int numFields);

    bool freeSpace() const;

    bool empty() const;

    int addEntry(const Type type, const int branchTargetAddress, const int architecturalRegister,
    const int physicalRegister, const int previousPhysicalRegister, const Instruction instruction);

    void execute();

    void resetEntry(const int index);

    void executingEntry(const int i);

    void finishedEntry(const int i, const int result);

    void writeResult(const int i, const int r);

    void flush();

    void printTail() const;

    void print() const;

  //getters and setters
  public:

    int getTailIndex() const;

    int getHeadIndex() const;

    void getReorderBufferInstructions(Instruction* const copy) const;

    void getReorderBufferFields(int** const copy) const;

    bool getFlushFlag() const;
};

#endif