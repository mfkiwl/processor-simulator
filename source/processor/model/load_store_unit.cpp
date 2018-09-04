//===========================================
//included header file containing interface
#include "load_store_unit.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

//==============================================================================================================
//public functions

LoadStoreUnit::LoadStoreUnit(Memory* const memory, ReorderBuffer* const reorderBuffer, 
  ALUReservationStation* const aluReservationStation) :
  memory(memory),
  reorderBuffer(reorderBuffer),
  nextLoadOpcode(0),
  nextLoadReorderBufferIndex(-1),
  nextStoreOpcode(0),
  nextStoreReorderBufferIndex(-1),
  loadOpcode(0),
  loadReorderBufferIndex(-1),
  storeOpcode(0),
  storeReorderBufferIndex(-1),
  bufferSize(20),
  writeCycles(5),
  readCycles(5),
  storeBuffer(memory, reorderBuffer, bufferSize, writeCycles),
  loadBuffer(memory, reorderBuffer, bufferSize, readCycles, aluReservationStation),
  aluReservationStation(aluReservationStation)
{
  //initially set all Operands to zero
  for(int i = 0; i < numOfOperands; i++) {
    nextLoadOperands[i] = 0;
    nextStoreOperands[i] = 0;
    loadOperands[i] = 0;
    storeOperands[i] = 0;
  }
}

void LoadStoreUnit::execute() {
  if(loadOpcode != NOOP) {

    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(loadReorderBufferIndex);

    //variables to hold temperary info
    int address;

    //execute the instruction
    switch(loadOpcode) {
      case LW:
      case LWR:
        address = 0 + loadOperands[1];
        //and to the read buffer to be read from memory when ready
        loadBuffer.addToBuffer(loadOperands[0], address, loadReorderBufferIndex);
        reorderBuffer->executingEntry(loadReorderBufferIndex);
        break;
    }
  }

  if(storeOpcode != NOOP) {

    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(storeReorderBufferIndex);

    //variables to hold temperary info
    int address;
    int value;

    //execute the instruction
    switch(storeOpcode) {
      case SW:
      case SWR:
        //get the address in memory to update and the value to update it to
        value = storeOperands[0];
        address = 0 + storeOperands[1];
        //and to the write buffer to be written to memory when ready
        storeBuffer.addToBuffer(address, value, storeReorderBufferIndex);
        reorderBuffer->executingEntry(storeReorderBufferIndex);
        break;
    }
  }

  //perform the read and write instructions when the step number has been met
  storeBuffer.writeIfReady();
  loadBuffer.readIfReady();

  //increment the step numbers for each inflight read and write instruction
  storeBuffer.incrementSteps();
  loadBuffer.incrementSteps();
}

void LoadStoreUnit::pipe() {

  //set the  values equal to the next values
  loadOpcode = nextLoadOpcode;
  storeOpcode = nextStoreOpcode;
  for(int i = 0; i < numOfOperands; i++) {
    loadOperands[i] = nextLoadOperands[i];
    storeOperands[i] = nextStoreOperands[i];
  }
  loadReorderBufferIndex = nextLoadReorderBufferIndex;
  storeReorderBufferIndex = nextStoreReorderBufferIndex;

  //reset the next values
  nextLoadOpcode = 0;
  nextStoreOpcode = 0;
  for(int i = 0; i < numOfOperands; i++) {
    nextLoadOperands[i] = 0;
    nextStoreOperands[i] = 0;
  }
  nextLoadReorderBufferIndex = -1;
  nextStoreReorderBufferIndex = -1;
}

void LoadStoreUnit::flush() {
  nextLoadOpcode = 0;
  nextStoreOpcode = 0;
  loadOpcode = 0;
  for(int i = 0; i < numOfOperands; i++) {
    nextLoadOperands[i] = 0;
    nextStoreOperands[i] = 0;
    loadOperands[i] = 0;
  }
  nextLoadReorderBufferIndex = -1;
  nextStoreReorderBufferIndex = -1;
  loadReorderBufferIndex = -1;
  storeBuffer.flush();
  loadBuffer.flush();
}

//===============================================================================================================
//getters and setters

void LoadStoreUnit::setNextLoadOpcode(const int x) {
  nextLoadOpcode = x;
}

void LoadStoreUnit::setNextLoadOperands(const int x[numOfOperands]) {
  for(int i = 0; i < numOfOperands; i++) {
    nextLoadOperands[i] = x[i];
  }
}

void LoadStoreUnit::setNextLoadReorderBufferIndex(const int i) {
  nextLoadReorderBufferIndex = i;
}

void LoadStoreUnit::setNextStoreOpcode(const int x) {
  nextStoreOpcode = x;
}

void LoadStoreUnit::setNextStoreOperands(const int x[numOfOperands]) {
  for(int i = 0; i < numOfOperands; i++) {
    nextStoreOperands[i] = x[i];
  }
}

void LoadStoreUnit::setNextStoreReorderBufferIndex(const int i) {
  nextStoreReorderBufferIndex = i;
}