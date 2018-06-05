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

  private:

    RegisterFile* const registerFile;
    Memory* const memory;

    int* const pc;
    int* const flushFlag;
    int* const runningFlag;

    int** buffer;
    const int size;
    int head;
    int tail;

    const int bufferEntryFields;

    //stores the original instruction issued
    Instruction* instructions;

    //no of instruction executed by the processor
    int* const noOfInstructionsExecuted;

  public:
    ReorderBuffer(RegisterFile* const registerFile, Memory* const memory, int* const pc, int* const flushFlag, int* const runningFlag, int* const noOfInstructionsExecuted);

    int addEntry(const Type type, const int destination, const Instruction instruction);

    int getTailIndex() const;

    void retire();

    void resetEntry(const int index);

    void executingEntry(const int i);

    void finishedEntry(const int i, const int result);

    void setEntryResult(const int i, const int r);

    void writeResult(const int i, const int r);

    void flush();

    void printTail() const;

    void print() const;
};

#endif