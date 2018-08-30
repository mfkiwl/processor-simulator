//===========================================
//included header file containing interface
#include "load_queue.h"

//===========================================
// included dependencies
#include "reorder_buffer.h"
#include "register_file.h"
#include "store_queue.h"
#include "load_store_unit.h"
#include "instructions.h"
#include "constants.h"

//==============================================================================================================
//public functions

LoadQueue::LoadQueue(ReorderBuffer* const reorderBuffer, RegisterFile* const registerFile, 
  StoreQueue* const storeQueue, LoadStoreUnit* const loadStoreUnit, int size) :
    reorderBuffer(reorderBuffer),
    registerFile(registerFile),
    storeQueue(storeQueue),
    loadStoreUnit(loadStoreUnit),
    size(size),
    nextInstructions(new Instruction[size]),
    nextReorderBufferIndexes(new int[size]),
    head(0),
    tail(0),
    instructions(new Instruction[size]),
    ages(new int[size]),
    validBits(new bool*[size]),
    reorderBufferIndexes(new int[size]),
    numReservedSpaces(0),
    dispatchIndex(-1)
    {
  	  //initialise arrays
  	  for(int i = 0; i < size; i++) {
  	    nextInstructions[i] = (Instruction) {0,0,0,0};
  	    nextReorderBufferIndexes[i] = -1;
  	    instructions[i] = (Instruction) {0,0,0,0};
  	    ages[i] = 0;
  	    validBits[i] = new bool[3];
  	    for(int j = 0; j < 3; j++) {
  	  	  validBits[i][j] = false;
  	    }
  	    reorderBufferIndexes[i] = -1;
  	  }
    }

void LoadQueue::execute() {
  incrementAges();
  checkOperandAvailability();
  //dispatch the instruction at the tail if is is ready, this results in all of the load and store
  //instruction being exeucted in order
  if(readyToDispatch(tail)) {
    dispatchIndex = tail;
    tail = (tail + 1) % size;
  }
}

void LoadQueue::pipe() {
  //send current instruction to the load store unit
  if(dispatchIndex != -1) {

    //send the decoded instruction to the execution unit
    loadStoreUnit->setNextLoadOpcode(instructions[dispatchIndex].opcode);
    loadStoreUnit->setNextLoadOperands(instructions[dispatchIndex].operands);
    //Send the reorder buffer index to the execution unit
    loadStoreUnit->setNextLoadReorderBufferIndex(reorderBufferIndexes[dispatchIndex]);

    //clear the dispatched instruction from the reservation station
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
    ages[dispatchIndex] = 0;
    for(int j = 0; j < 3; j++) {
      validBits[dispatchIndex][j] = false;
    }
    reorderBufferIndexes[dispatchIndex] = -1;
    dispatchIndex = -1;
  }
  //add the next instruction to the buffer
  addNextInstructions();
  //clear the nextInstruction and nextReorderBufferIndex
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
}

void LoadQueue::flush() {
  head = 0;
  tail = 0;
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    ages[i] = 0;
    for(int j = 0; j < 3; j++) {
      validBits[i][j] = false;
    }
    reorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
}

void LoadQueue::print() const {
  printf("Load/Store Unit reservation station:\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}

bool LoadQueue::freeSpace() const {
  int count = 0;
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      count++;
    }
  }
  if(dispatchIndex != -1) {
    count--;
  }
  count += numReservedSpaces;
  if(count == size) {
    return false;
  }
  else {
    return true;
  }
}

void LoadQueue::reserveSpace() {
  numReservedSpaces++;
}

void LoadQueue::broadcast(int physicalRegister, int value) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case LW:
        break;
      case LWR:
        if(!validBits[i][1] && instructions[i].operands[1] == physicalRegister) {
          instructions[i].operands[1] = value;
          validBits[i][1] = true;
        }
        break;
    }
  }
}


//====================================================================================================================
//private functions

void LoadQueue::incrementAges() {
  for(int i = 0; i < size; i++) {
  	if(instructions[i].opcode != NOOP) {
  	  ages[i]++;
  	}
  }
}

void LoadQueue::checkOperandAvailability() {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case LW:
        break;
      case LWR:
        if(!validBits[i][1]) {
          if(registerFile->getScoreBoardValue(instructions[i].operands[1])) {
            instructions[i].operands[1] = registerFile->getPhysicalRegisterValue(instructions[i].operands[1]);
            validBits[i][1] = true;
          }
        }
        break;
    }
  }
}

bool LoadQueue::readyToDispatch(const int index) const {
  //check that the source register are ready to use
  switch(instructions[index].opcode) {
    case NOOP:
      return false;
    case LW:
      if(storeQueue->checkLoad(ages[index], instructions[index].operands[1])) {
        //ready
        return true;
      }
    break;
    case LWR:
      //ready is the source registers are ready
      if(validBits[index][1] && storeQueue->checkLoad(ages[index], instructions[index].operands[1])) {
        return true;
      }
      break;
  }
  return false;
}

void LoadQueue::addNextInstructions() {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode != NOOP) {
      instructions[head] = nextInstructions[i];
      reorderBufferIndexes[head] = nextReorderBufferIndexes[i];
      head = (head + 1) % size;
    }
  }
}

//=============================================================================================
//getters and setters

void LoadQueue::getCurrentInstructions(Instruction* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void LoadQueue::getCurrentReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void LoadQueue::setNextInstruction(const Instruction instruction, const int rbi) {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode == NOOP) {
      nextInstructions[i] = instruction;
      nextReorderBufferIndexes[i] = rbi;
      break;
    }
  }
}

void LoadQueue::getValidBits(bool** const copy) const {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < 3; j++) {
      copy[i][j] = validBits[i][j];
    }
  }
}