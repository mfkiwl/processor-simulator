//===========================================
//included header file containing interface
#include "load_store_unit_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "register_file.h"
#include "reorder_buffer.h"
#include "load_store_unit.h"
#include "instructions.h"

//===========================================
//class implementation

LoadStoreUnitReservationStation::LoadStoreUnitReservationStation(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, LoadStoreUnit* const loadStoreUnit, const int size) : 
  registerFile(registerFile),
  reorderBuffer(reorderBuffer),
  loadStoreUnit(loadStoreUnit),
  loadQueueNextInstruction((Instruction) {0,0,0,0}),
  loadQueueNextReorderBufferIndex(-1),
  loadQueueTail(0),
  loadQueueHead(0),
  loadQueueSize(size),
  loadQueueInstructions(new Instruction[loadQueueSize]),
  loadQueueAges(new int[loadQueueSize]),
  loadQueueReorderBufferIndexes(new int[loadQueueSize]),
  storeQueueNextInstruction((Instruction) {0,0,0,0}),
  storeQueueNextReorderBufferIndex(-1),
  storeQueueTail(0),
  storeQueueHead(0),
  storeQueueSize(size),
  storeQueueInstructions(new Instruction[storeQueueSize]),
  storeQueueAges(new int[loadQueueSize]),
  storeQueueReorderBufferIndexes(new int[storeQueueSize]),
  opcode(0),
  reorderBufferIndex(-1),
  dispatchIndex(-1)
{
  //set all instructions to NOOPs and all reorder buffer indexes to -1
  for(int i = 0; i < loadQueueSize; i++) {
    loadQueueInstructions[i] = (Instruction) {0,0,0,0};
    loadQueueReorderBufferIndexes[i] = -1;
    loadQueueAges[i] = 0;
  }
  for(int i = 0; i < storeQueueSize; i++) {
    storeQueueInstructions[i] = (Instruction) {0,0,0,0};
    storeQueueReorderBufferIndexes[i] = -1;
    storeQueueAges[i] = 0;
  }
  //zero out all operands
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
}

bool LoadStoreUnitReservationStation::readyToDispatch(const int index) const {
  Instruction instruction = loadQueueInstructions[index];
  //check that the source register are ready to use
  switch(instruction.opcode) {
    case NOOP:
      return false;
    case LW:
      if(!loadStoreUnit->waitingForStore()) {
        //ready
        return true;
      }
    break;
    case SW:
      //ready if the source registers are ready and the instruction is at the ROB tail
      if(registerFile->getScoreBoardValue(operands[0]) && loadQueueReorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return true;
      }
      break;
    case LWR:
      //ready is the source registers are ready
      if(!loadStoreUnit->waitingForStore() && registerFile->getScoreBoardValue(operands[1])) {
        return true;
      }
      break;
    case SWR:
      //ready if the source registers are ready and the instruction is at the ROB tail
      if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1]) && loadQueueReorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return true;
      }
      break;
  }
  return false;
}

//dispatch bound fetch
void LoadStoreUnitReservationStation::fetchOperands(const int index) {
  Instruction instruction = loadQueueInstructions[index];
  //getting the opcode and incomplete operands from the instruction
  opcode = instruction.opcode;
  for(int i = 0; i < 3; i++) {
    operands[i] = instruction.operands[i];
  }
  //temp variables
  int registerNum;
  int val; 
  //fetching the operands for the instruction
  switch(opcode) {
    case NOOP:
      break;
    case LW:
      break;
    case SW:
      registerNum = operands[0];
      val = registerFile->getRegisterValue(registerNum);
      operands[0] = val;
      break;
    case LWR:
      //If the source registers are ready then continue
      registerNum = operands[1];
      val = registerFile->getRegisterValue(registerNum);
      operands[1] = val;
      break;
    case SWR:
      //If the source registers are ready then continue
      registerNum = operands[0];
      val = registerFile->getRegisterValue(registerNum);
      operands[0] = val;
      registerNum = operands[1];
      val = registerFile->getRegisterValue(registerNum);
      operands[1] = val;
      break;
  }
}

void LoadStoreUnitReservationStation::incrementAges() {
  for(int i = 0; i < loadQueueSize; i++) {
    if(loadQueueInstructions[i].opcode != NOOP) {
      loadQueueAges[i]++;
    }
  }
  for(int i = 0; i < storeQueueSize; i++) {
    if(storeQueueInstructions[i].opcode != NOOP) {
      storeQueueAges[i]++;
    }
  }
}

void LoadStoreUnitReservationStation::execute() {
  //dispatch the instruction at the tail if is is ready, this results in all of the load and store
  //instruction being exeucted in order
  if(loadQueueInstructions[loadQueueTail].opcode != NOOP) {
    if(readyToDispatch(loadQueueTail)) {
      fetchOperands(loadQueueTail);
      reorderBufferIndex = loadQueueReorderBufferIndexes[loadQueueTail];
      dispatchIndex = loadQueueTail;
      loadQueueTail = (loadQueueTail + 1) % loadQueueSize;
    }
  }
  incrementAges();
}

void LoadStoreUnitReservationStation::addInstruction(const Instruction instruction, const int rbi) {
  if(instruction.opcode != NOOP) {
    loadQueueInstructions[loadQueueHead] = instruction;
    loadQueueReorderBufferIndexes[loadQueueHead] = rbi;
    loadQueueAges[loadQueueHead] = 0;
    loadQueueHead = (loadQueueHead + 1) % loadQueueSize;
  }
}

bool LoadStoreUnitReservationStation::spaceInQueue() const {
  if(loadQueueTail == loadQueueHead && loadQueueInstructions[loadQueueHead].opcode != NOOP) {
    return false;
  }
  else {
    return true;
  }
}

void LoadStoreUnitReservationStation::pipe() {
  //send current instruction to the load store unit
  if(reorderBufferIndex != -1) {

    //clear the dispatched instruction from the reservation station
    loadQueueInstructions[dispatchIndex] = (Instruction) {0,0,0,0};
    loadQueueReorderBufferIndexes[dispatchIndex] = -1;
    loadQueueAges[dispatchIndex] = 0;
    dispatchIndex = -1;

    //send the decoded instruction to the execution unit
    loadStoreUnit->setNextOpcode(opcode);
    loadStoreUnit->setNextOperands(operands);

    //Send the reorder buffer index to the execution unit
    loadStoreUnit->setNextReorderBufferIndex(reorderBufferIndex);
        
    //reset the outputs
    opcode = 0;
    for(int i = 0; i < 3; i++) {
      operands[i] = 0;
    }
    reorderBufferIndex = -1;
  }
  //add the next instruction to the buffer
  addInstruction(loadQueueNextInstruction, loadQueueNextReorderBufferIndex);
  //clear the nextInstruction and nextReorderBufferIndex
  loadQueueNextInstruction = (Instruction) {0,0,0,0};
  loadQueueNextReorderBufferIndex = -1;
}

void LoadStoreUnitReservationStation::flush() {
  //reset the load queue
  loadQueueTail = 0;
  loadQueueHead = 0;
  for(int i = 0; i < loadQueueSize; i++) {
    loadQueueInstructions[i] = (Instruction) {0,0,0,0};
    loadQueueReorderBufferIndexes[i] = -1;
    loadQueueAges[i] = 0;
  }
  loadQueueNextInstruction = (Instruction) {0,0,0,0};
  loadQueueNextReorderBufferIndex = -1;
  //reset the store queue
  storeQueueTail = 0;
  storeQueueHead = 0;
  for(int i = 0; i < storeQueueSize; i++) {
    storeQueueInstructions[i] = (Instruction) {0,0,0,0};
    storeQueueReorderBufferIndexes[i] = -1;
    storeQueueAges[i] = 0;
  }
  storeQueueNextInstruction = (Instruction) {0,0,0,0};
  storeQueueNextReorderBufferIndex = -1;
  //reset info to be piped
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
  reorderBufferIndex = -1;
}

void LoadStoreUnitReservationStation::print() const {
  printf("Load/Store Unit reservation station:\n");
  for(int i = 0; i < loadQueueSize; i++) {
    if(loadQueueInstructions[i].opcode != NOOP) {
      printInstruction(loadQueueInstructions[i]);
    }
  }
}

void LoadStoreUnitReservationStation::getLoadQueueInstructions(Instruction* const copy) const {
  for(int i = 0; i < loadQueueSize; i++) {
    copy[i] = loadQueueInstructions[i];
  }
}

void LoadStoreUnitReservationStation::getLoadQueueReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < loadQueueSize; i++) {
    copy[i] = loadQueueReorderBufferIndexes[i];
  }
}

void LoadStoreUnitReservationStation::getLoadQueueAges(int* const copy) const {
  for(int i = 0; i < loadQueueSize; i++) {
    copy[i] = loadQueueAges[i];
  }
}

void LoadStoreUnitReservationStation::getStoreQueueInstruction(Instruction* const copy) const {
  for(int i = 0; i < storeQueueSize; i++) {
    copy[i] = storeQueueInstructions[i];
  }
}

void LoadStoreUnitReservationStation::getStoreQueueReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < storeQueueSize; i++) {
    copy[i] = storeQueueReorderBufferIndexes[i];
  }
}

void LoadStoreUnitReservationStation::getStoreQueueAges(int* const copy) const {
  for(int i = 0; i < storeQueueSize; i++) {
    copy[i] = storeQueueAges[i];
  }
}

void LoadStoreUnitReservationStation::setLoadQueueNextInstruction(const Instruction instruction) {
  loadQueueNextInstruction = instruction;
}

void LoadStoreUnitReservationStation::setLoadQueueNextReorderBufferIndex(const int index) {
  loadQueueNextReorderBufferIndex = index;
}

void LoadStoreUnitReservationStation::setStoreQueueNextInstruction(const Instruction instruction) {
  storeQueueNextInstruction = instruction;
}

void LoadStoreUnitReservationStation::setStoreQueueNextReorderBufferIndex(const int index) {
  storeQueueNextReorderBufferIndex = index;
}