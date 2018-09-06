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
    nextOperandTypes(new OperandType*[size]),
    nextReorderBufferIndexes(new int[size]),
    head(0),
    tail(0),
    instructions(new Instruction[size]),
    operandTypes(new OperandType*[size]),
    ages(new int[size]),
    reorderBufferIndexes(new int[size]),
    numReservedSpaces(0),
    dispatchIndex(-1)
    {
  	  //initialise arrays
  	  for(int i = 0; i < size; i++) {
  	    nextInstructions[i] = (Instruction) {0,0,0,0};
        nextOperandTypes[i] = new OperandType[numOfOperands];
        for(int j = 0; j < numOfOperands; j++) {
          nextOperandTypes[i][j] = NONE;
        }
  	    nextReorderBufferIndexes[i] = -1;
  	    instructions[i] = (Instruction) {0,0,0,0};
        operandTypes[i] = new OperandType[numOfOperands];
        for(int j = 0; j < numOfOperands; j++) {
          operandTypes[i][j] = NONE;
        }
  	    ages[i] = 0;
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
    for(int j = 0; j < numOfOperands; j++) {
      operandTypes[dispatchIndex][j] = NONE;
    }
    ages[dispatchIndex] = 0;
    reorderBufferIndexes[dispatchIndex] = -1;
    dispatchIndex = -1;
  }
  //add the next instruction to the buffer
  addNextInstructions();
  //clear the nextInstruction and nextReorderBufferIndex
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < numOfOperands; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
}

void LoadQueue::flush() {
  head = 0;
  tail = 0;
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < numOfOperands; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    for(int j=  0; j < numOfOperands; j++) {
      operandTypes[i][j] = NONE;
    }
    ages[i] = 0;
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

void LoadQueue::broadcast(int robEntry, int value) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case LW:
        break;
      case LWR:
        if((operandTypes[i][1] == ROB) && instructions[i].operands[1] == robEntry) {
          instructions[i].operands[1] = value;
          operandTypes[i][1] = CONSTANT;
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
        if(!(operandTypes[i][1] == CONSTANT)) {
          if(operandTypes[i][1] == REGISTER) {
            instructions[i].operands[1] = registerFile->getRegisterValue(instructions[i].operands[1]);
            operandTypes[i][1] = CONSTANT;
          }
          else if(operandTypes[i][1] == ROB) {
            if(reorderBuffer->isEntryFinished(instructions[i].operands[1])) {
              instructions[i].operands[1] = reorderBuffer->getEntryResult(instructions[i].operands[1]);
              operandTypes[i][1] = CONSTANT;
            }
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
      if((operandTypes[index][1] == CONSTANT) && storeQueue->checkLoad(ages[index], instructions[index].operands[1])) {
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
      for(int j = 0; j < numOfOperands; j++) {
        operandTypes[head][j] = nextOperandTypes[i][j];
      }
      reorderBufferIndexes[head] = nextReorderBufferIndexes[i];
      head = (head + 1) % size;
    }
  }
}

//=============================================================================================
//getters and setters

void LoadQueue::setNextInstruction(const Instruction instruction, const OperandType types[], const int rbi) {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode == NOOP) {
      nextInstructions[i] = instruction;
      for(int j = 0; j < numOfOperands; j++) {
        nextOperandTypes[i][j] = types[j];
      }
      nextReorderBufferIndexes[i] = rbi;
      break;
    }
  }
}

void LoadQueue::getCurrentInstructions(Instruction copy[]) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void LoadQueue::getCurrentReorderBufferIndexes(int copy[]) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void LoadQueue::getOperandTypes(OperandType copy[][numOfOperands]) const {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < numOfOperands; j++) {
      copy[i][j] = operandTypes[i][j];
    }
  }
}