//===========================
//include guard
#ifndef LOAD_QUEUE_H
#define LOAD_QUEUE_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;
class StoreQueue;
class LoadStoreUnit;
class Instruction;

//=================================
// included dependencies

//===========================
//class declaration
class LoadQueue {

  //private member variables
  private:

  	ReorderBuffer* const reorderBuffer;
  	RegisterFile* const registerFile;
  	StoreQueue* const storeQueue;
  	LoadStoreUnit* const loadStoreUnit;

  	const int size;

  	Instruction* const nextInstructions;
  	int* const nextReorderBufferIndexes;

  	int head;
  	int tail;

  	Instruction* const instructions;
  	int* const ages;
  	bool** const validBits;
  	int* const reorderBufferIndexes;

  	int numReservedSpaces;

  	int dispatchIndex;


  //public functions
  public:

  	LoadQueue(ReorderBuffer* const reorderBuffer, RegisterFile* const registerFile, StoreQueue* const storeQueue, 
  	  LoadStoreUnit* const loadStoreUnit, int size);

  	void execute();

    void pipe();

    void flush();

    void print() const;

  	bool freeSpace() const;

    void reserveSpace();

  	void broadcast(int physicalRegister, int value);

  //private functions
  private:

  	void checkOperandAvailability();

  	bool readyToDispatch(const int index) const;

  	void addNextInstructions();

  //getters and setters
  public:

    void getCurrentInstructions(Instruction* const copy) const;

    void getCurrentReorderBufferIndexes(int* const copy) const;

    void setNextInstruction(const Instruction instruction, const int rbi);
};

#endif