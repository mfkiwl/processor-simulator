//===========================
//include guard
#ifndef STORE_QUEUE_H
#define STORE_QUEUE_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;
class LoadStoreUnit;
class LoadQueue;
class Instruction;

//=================================
// included dependencies

#include "constants.h"

//===========================
//class declaration
class StoreQueue {

  //private member variables
  private:

  	ReorderBuffer* const reorderBuffer;
  	RegisterFile* const registerFile;
  	LoadStoreUnit* const loadStoreUnit;

  	const int size;

  	Instruction* const nextInstructions;
    OperandType** const nextOperandTypes;
  	int* const nextReorderBufferIndexes;

  	int head;
  	int tail;

  	Instruction* const instructions;
    OperandType** const operandTypes;
    int* const ages;
  	bool** const validBits;
  	int* const reorderBufferIndexes;

  	int numReservedSpaces;

  	int dispatchIndex;


  //public functions
  public:

  	StoreQueue(ReorderBuffer* const reorderBuffer, RegisterFile* const registerFile, LoadStoreUnit* const loadStoreUnit, 
      const int size);

  	void execute();

    void pipe();

    void flush();

    void print() const;

  	bool freeSpace() const;

    void reserveSpace();

  	void broadcast(int physicalRegister, int value);

    bool checkLoad(int age, int address);

  //private functions
  private:

    void incrementAges();

  	void checkOperandAvailability();

  	bool readyToDispatch(const int index) const;

  	void addNextInstructions();

  //getters and setters
  public:

    int getTailAge() const;

    void setNextInstruction(const Instruction instruction, const OperandType types[], const int rbi);

    void getCurrentInstructions(Instruction copy[]) const;

    void getCurrentReorderBufferIndexes(int copy[]) const;

    void getValidBits(bool copy[][3]) const;

    void getOperandTypes(OperandType copy[][3]) const;
};

#endif