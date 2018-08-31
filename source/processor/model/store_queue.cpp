//===========================================
//included header file containing interface
#include "store_queue.h"

//===========================================
// included dependencies
#include "reorder_buffer.h"
#include "register_file.h"
#include "load_queue.h"
#include "load_store_unit.h"
#include "instructions.h"
#include "constants.h"

//==============================================================================================================
//public functions

StoreQueue::StoreQueue(ReorderBuffer* const reorderBuffer, RegisterFile* const registerFile, 
  LoadStoreUnit* const loadStoreUnit, const int size) :
    reorderBuffer(reorderBuffer),
    registerFile(registerFile),
    loadStoreUnit(loadStoreUnit),
    size(size),
    nextInstructions(new Instruction[size]),
    nextOperandTypes(new int*[size]),
    nextReorderBufferIndexes(new int[size]),
    head(0),
    tail(0),
    instructions(new Instruction[size]),
    operandTypes(new int*[size]),
    ages(new int[size]),
    validBits(new bool*[size]),
    reorderBufferIndexes(new int[size]),
    numReservedSpaces(0),
    dispatchIndex(-1)
    {
  	  //initialise arrays
  	  for(int i = 0; i < size; i++) {
  	    nextInstructions[i] = (Instruction) {0,0,0,0};
        nextOperandTypes[i] = new int[3];
        for(int j = 0; j < 3; j++) {
          nextOperandTypes[i][j] = NONE;
        }
  	    nextReorderBufferIndexes[i] = -1;
  	    instructions[i] = (Instruction) {0,0,0,0};
        operandTypes[i] = new int[3];
        for(int j = 0; j < 3; j++) {
          operandTypes[i][j] = NONE;
        }
        ages[i] = 0;
  	    validBits[i] = new bool[3];
  	    for(int j = 0; j < 3; j++) {
  	  	  validBits[i][j] = false;
  	    }
  	    reorderBufferIndexes[i] = -1;
  	  }
    }

void StoreQueue::execute() {
  incrementAges();
  checkOperandAvailability();
  //dispatch the instruction at the tail if is is ready, this results in all of the load and store
  //instruction being exeucted in order
  if(readyToDispatch(tail)) {
    dispatchIndex = tail;
    tail = (tail + 1) % size;
  }
}

void StoreQueue::pipe() {
  //send current instruction to the load store unit
  if(dispatchIndex != -1) {

    //send the decoded instruction to the execution unit
    loadStoreUnit->setNextStoreOpcode(instructions[dispatchIndex].opcode);
    loadStoreUnit->setNextStoreOperands(instructions[dispatchIndex].operands);
    //Send the reorder buffer index to the execution unit
    loadStoreUnit->setNextStoreReorderBufferIndex(reorderBufferIndexes[dispatchIndex]);

    //clear the dispatched instruction from the reservation station
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
    for(int j = 0; j < 3; j++) {
      operandTypes[dispatchIndex][j] = NONE;
    }
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
    for(int j = 0; j < 3; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
}

void StoreQueue::flush() {
  head = 0;
  tail = 0;
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < 3; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < 3; j++) {
      operandTypes[i][j] = NONE;
    }
    ages[i] = 0;
    for(int j = 0; j < 3; j++) {
      validBits[i][j] = false;
    }
    reorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
}

void StoreQueue::print() const {
  printf("Load/Store Unit reservation station:\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}

bool StoreQueue::freeSpace() const {
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

void StoreQueue::reserveSpace() {
  numReservedSpaces++;
}

void StoreQueue::broadcast(int physicalRegister, int value) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case SW:
        if(!validBits[i][0] && instructions[i].operands[0] == physicalRegister) {
          instructions[i].operands[0] = value;
          validBits[i][0] = true;
        }
        break;
      case SWR:
        if(!validBits[i][0] && instructions[i].operands[0] == physicalRegister) {
          instructions[i].operands[0] = value;
          validBits[i][0] = true;
        }
        if(!validBits[i][1] && instructions[i].operands[1] == physicalRegister) {
          instructions[i].operands[1] = value;
          validBits[i][1] = true;
        }
        break;
    }
  }
}

bool StoreQueue::checkLoad(int age, int address) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case SW:
        if(ages[i] > age && instructions[i].operands[1] == address) {
          return false;
        }
        break;
      case SWR:
        if(ages[i] > age) {
          if(!validBits[i][1]) {
            return false;
          }
          else if(instructions[i].operands[1] == address) {
            return false;
          }
        }
        break;
    }
  }
  return true;
}


//====================================================================================================================
//private functions

void StoreQueue::incrementAges() {
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      ages[i]++;
    }
  }
}

void StoreQueue::checkOperandAvailability() {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case SW:
        if(!validBits[i][0]) {
          if(registerFile->getScoreBoardValue(instructions[i].operands[0])) {
            instructions[i].operands[0] = registerFile->getPhysicalRegisterValue(instructions[i].operands[0]);
            validBits[i][0] = true;
          }
        }
        break;
      case SWR:
        if(!validBits[i][0]) {
          if(registerFile->getScoreBoardValue(instructions[i].operands[0])) {
            instructions[i].operands[0] = registerFile->getPhysicalRegisterValue(instructions[i].operands[0]);
            validBits[i][0] = true;
          }
        }
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

bool StoreQueue::readyToDispatch(const int index) const {
  //check that the source register are ready to use
  switch(instructions[index].opcode) {
    case NOOP:
      return false;
    case SW:
      //ready if the source registers are ready and the instruction is at the ROB tail
      if(validBits[index][0] && reorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return true;
      }
      break;
    case SWR:
      //ready if the source registers are ready and the instruction is at the ROB tail
      if(validBits[index][0] && validBits[index][1] && reorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return true;
      }
      break;
  }
  return false;
}

void StoreQueue::addNextInstructions() {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode != NOOP) {
      instructions[head] = nextInstructions[i];
      for(int j = 0; j < 3; j++) {
        operandTypes[head][j] = nextOperandTypes[i][j];
      }
      ages[head]++;
      reorderBufferIndexes[head] = nextReorderBufferIndexes[i];
      head = (head + 1) % size;
    }
  }
}

//=============================================================================================
//getters and setters

int StoreQueue::getTailAge() const {
  return ages[tail];
}

void StoreQueue::getCurrentInstructions(Instruction* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void StoreQueue::getCurrentReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void StoreQueue::setNextInstruction(const Instruction instruction, const int types[], const int rbi) {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode == NOOP) {
      nextInstructions[i] = instruction;
      for(int j = 0; j < 3; j++) {
        nextOperandTypes[i][j] = types[j];
      }
      nextReorderBufferIndexes[i] = rbi;
      break;
    }
  }
}

void StoreQueue::getValidBits(bool** const copy) const {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < 3; j++) {
      copy[i][j] = validBits[i][j];
    }
  }
}