//===========================================
//included header file containing interface
#include "load_store_unit.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

//===========================================
//class implementation

LoadStoreUnit::LoadStoreUnit(Memory* const memory, ReorderBuffer* const reorderBuffer) :
  memory(memory),
  reorderBuffer(reorderBuffer),
  nextOpcode(0),
  currentOpcode(0),
  bufferSize(100),
  writeCycles(5),
  readCycles(5),
  storeBuffer(memory, reorderBuffer, bufferSize, writeCycles),
  loadBuffer(memory, reorderBuffer, bufferSize, readCycles),
  currentReorderBufferIndex(-1)
{
  //initially set all currentOperands to zero
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    currentOperands[i] = 0;
  }
}

void LoadStoreUnit::execute() {
  if(currentOpcode != NOOP) {

    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(currentReorderBufferIndex);

    //variables to hold temperary info
    int address;
    int value;

    //execute the instruction
    switch(currentOpcode) {
      case LW:
      case LWR:
        address = 0 + currentOperands[1];
        //and to the read buffer to be read from memory when ready
        loadBuffer.addToBuffer(currentOperands[0], address, currentReorderBufferIndex);
        reorderBuffer->executingEntry(currentReorderBufferIndex);
        break;
      case SW:
      case SWR:
        //get the address in memory to update and the value to update it to
        value = currentOperands[0];
        address = 0 + currentOperands[1];
        //and to the write buffer to be written to memory when ready
        storeBuffer.addToBuffer(address, value, currentReorderBufferIndex);
        break;
    }
  }

  //increment the step numbers for each inflight read and write instruction
  storeBuffer.stepInstructions();
  loadBuffer.stepInstructions();
}

void LoadStoreUnit::pipe() {
  //perform the read and write instructions when the step number has been met
  storeBuffer.writeIfReady();
  loadBuffer.readIfReady();

  //set the current values equal to the next values
  currentOpcode = nextOpcode;
  for(int i = 0; i < 3; i++) {
    currentOperands[i] = nextOperands[i];
  }
  currentReorderBufferIndex = nextReorderBufferIndex;

  //reset the next values
  nextOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

void LoadStoreUnit::setNextOpcode(const int x) {
  nextOpcode = x;
}

void LoadStoreUnit::setNextOperands(const int x[3]) {
  for(int i = 0; i < 3; i++) {
   nextOperands[i] = x[i];
  }
}

void LoadStoreUnit::setNextReorderBufferIndex(const int i) {
  nextReorderBufferIndex = i;
}

void LoadStoreUnit::flush() {
  nextOpcode = 0;
  currentOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    currentOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  currentReorderBufferIndex = -1;
  storeBuffer.flush();
  loadBuffer.flush();
}

int LoadStoreUnit::waitingForStore() const {
  return storeBuffer.waitingForStore();
}