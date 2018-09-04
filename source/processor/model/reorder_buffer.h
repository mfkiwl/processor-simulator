//===========================
//include guard
#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

//===========================
//forward declared dependencies
class RegisterFile;
class Memory;
class Instruction;
class FetchUnit;

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
    FetchUnit* const fetchUnit;

    int* const pc;
    bool flushFlag;
    bool* const runningFlag;

    const int bufferSize;
    int** const buffer;
    int head;
    int tail;

    //stores the original instruction issued
    Instruction* const instructions;

    //no of instruction executed by the processor
    int* const noOfInstructionsExecuted;

    const int issueWindowSize;

  //public functions
  public:

    ReorderBuffer(RegisterFile* const registerFile, Memory* const memory, FetchUnit* const fetchUnit, int* const pc, 
      bool* const runningFlag, int* const noOfInstructionsExecuted, const int bufferSize, const int issueWindowSize);

    bool freeSpace() const;

    bool empty() const;

    int addEntry(const Type type, const bool branchPrediction, const int branchTargetAddress, 
      const int architecturalRegister, const int physicalRegister, const int previousPhysicalRegister, 
      const Instruction instruction);

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

    void getReorderBufferInstructions(Instruction copy[]) const;

    void getReorderBufferFields(int copy[][ReorderBufferIndex::COUNT]) const;

    bool getFlushFlag() const;
};

#endif