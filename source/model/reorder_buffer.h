//===========================
//include guard
#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

//===========================
//forward declared dependencies
class RegisterFile;
class Memory;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class ReorderBuffer {

    RegisterFile* registerFile;
    Memory* memory;

    int* pc;
    int* flushFlag;
    int* runningFlag;

    int** buffer;
    int size;
    int head;
    int tail;

    int bufferEntryFields;

    //stores the original instruction issued
    Instruction* instructions;

    //no of instruction executed by the processor
    int* noOfInstructionsExecuted;

  public:
    ReorderBuffer(RegisterFile* registerFile, Memory* memory, int* pc, int* flushFlag, int* runningFlag, int* noOfInstructionsExecuted);

    int addEntry(Type type, int destination, Instruction instruction);

    int getTailIndex();

    void retire();

    void resetEntry(int index);

    void executingEntry(int i);

    void finishedEntry(int i, int result);

    void setEntryResult(int i, int r);

    void writeResult(int i, int r);

    void flush();

    void printTail();

    void print();
};

#endif