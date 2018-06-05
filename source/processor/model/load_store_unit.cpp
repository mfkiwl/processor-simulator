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
  opcode(0),
  bufferSize(100),
  writeCycles(5),
  readCycles(5),
  storeBuffer(memory, reorderBuffer, bufferSize, writeCycles),
  loadBuffer(memory, reorderBuffer, bufferSize, readCycles),
  reorderBufferIndex(-1)
{
  //initially set all operands to zero
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
}

void LoadStoreUnit::execute() {
  if(reorderBufferIndex != -1) {

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
        break;
    }

    //reset the variables
    opcode = 0;
    for(int i = 0; i < 3; i++) {
      operands[i] = 0;
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
}

void LoadStoreUnit::setOpcode(const int x) {
  opcode = x;
}

void LoadStoreUnit::setOperands(const int x[3]) {
  for(int i = 0; i < 3; i++) {
    operands[i] = x[i];
  }
}

void LoadStoreUnit::flush() {
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
  reorderBufferIndex = -1;
  storeBuffer.flush();
  loadBuffer.flush();
}

void LoadStoreUnit::setReorderBufferIndex(const int i) {
  reorderBufferIndex = i;
}

int LoadStoreUnit::waitingForStore() const {
  return storeBuffer.waitingForStore();
}