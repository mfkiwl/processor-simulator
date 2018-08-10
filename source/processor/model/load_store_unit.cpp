//===========================================
//included header file containing interface
#include "load_store_unit.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

//==============================================================================================================
//public functions

LoadStoreUnit::LoadStoreUnit(Memory* const memory, ReorderBuffer* const reorderBuffer) :
  memory(memory),
  reorderBuffer(reorderBuffer),
  nextOpcode(0),
  nextReorderBufferIndex(-1),
  opcode(0),
  reorderBufferIndex(-1),
  bufferSize(20),
  writeCycles(5),
  readCycles(5),
  storeBuffer(memory, reorderBuffer, bufferSize, writeCycles),
  loadBuffer(memory, reorderBuffer, bufferSize, readCycles)
{
  //initially set all Operands to zero
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
}

void LoadStoreUnit::execute() {
  if(opcode != NOOP) {

    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(reorderBufferIndex);

    //variables to hold temperary info
    int address;
    int value;

    //execute the instruction
    switch(opcode) {
      case LW:
      case LWR:
        address = 0 + operands[1];
        //and to the read buffer to be read from memory when ready
        loadBuffer.addToBuffer(operands[0], address, reorderBufferIndex);
        reorderBuffer->executingEntry(reorderBufferIndex);
        break;
      case SW:
      case SWR:
        //get the address in memory to update and the value to update it to
        value = operands[0];
        address = 0 + operands[1];
        //and to the write buffer to be written to memory when ready
        storeBuffer.addToBuffer(address, value, reorderBufferIndex);
        reorderBuffer->executingEntry(reorderBufferIndex);
        break;
    }
  }

  //increment the step numbers for each inflight read and write instruction
  storeBuffer.incrementSteps();
  loadBuffer.incrementSteps();
}

void LoadStoreUnit::pipe() {
  //perform the read and write instructions when the step number has been met
  storeBuffer.writeIfReady();
  loadBuffer.readIfReady();

  //set the  values equal to the next values
  opcode = nextOpcode;
  for(int i = 0; i < 3; i++) {
    operands[i] = nextOperands[i];
  }
  reorderBufferIndex = nextReorderBufferIndex;

  //reset the next values
  nextOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

void LoadStoreUnit::flush() {
  nextOpcode = 0;
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  reorderBufferIndex = -1;
  storeBuffer.flush();
  loadBuffer.flush();
}

//===============================================================================================================
//getters and setters

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