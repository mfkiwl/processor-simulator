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
  nextInstruction((Instruction) {0,0,0,0}),
  nextReorderBufferIndex(-1),
  tail(0),
  head(0),
  size(size),
  instructions(new Instruction[size]),
  reorderBufferIndexes(new int[size]),
  opcode(0),
  reorderBufferIndex(-1)
{
  //set all instructions to NOOPs
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
  }
  //set all reorder buffer indexes to -1
  for(int i = 0; i < size; i++) {
    reorderBufferIndexes[i] = -1;
  }
  //zero out all operands
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
}

void LoadStoreUnitReservationStation::execute() {
  //try and find an instruction that can be dispatched
  if(instructions[tail].opcode != NOOP) {
    if(readyToDispatch(tail)) {
      dispatch(tail);
      reorderBufferIndex = reorderBufferIndexes[tail];

      //clear the dispatched instruction from the reservation station
      instructions[tail] = (Instruction) {0,0,0,0};
      tail = (tail + 1) % size;
    }
  }
}

void LoadStoreUnitReservationStation::addInstruction(const Instruction instruction, const int rbi) {
  if(instruction.opcode != NOOP) {
    instructions[head] = instruction;
    reorderBufferIndexes[head] = rbi;
    head = (head + 1) % size;
  }
}

int LoadStoreUnitReservationStation::spaceInQueue() const {
  if(tail == head && instructions[head].opcode != NOOP) {
    return 0;
  }
  else {
    return 1;
  }
}

void LoadStoreUnitReservationStation::pipe() {
  //send current instruction to the load store unit
  if(reorderBufferIndex != -1) {

    //send the decoded instruction to the execution unit
    loadStoreUnit->setOpcode(opcode);
    loadStoreUnit->setOperands(operands);

    //Send the reorder buffer index to the execution unit
    loadStoreUnit->setReorderBufferIndex(reorderBufferIndex);
        
    //reset the outputs
    opcode = 0;
    for(int i = 0; i < 3; i++) {
      operands[i] = 0;
    }
    reorderBufferIndex = -1;
  }
  //add the next instruction to the buffer
  addInstruction(nextInstruction, nextReorderBufferIndex);
  //clear the nextInstruction and nextReorderBufferIndex
  nextInstruction = (Instruction) {0,0,0,0};
  nextReorderBufferIndex = -1;
}

void LoadStoreUnitReservationStation::flush() {
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
  }
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
  reorderBufferIndex = -1;
}

void LoadStoreUnitReservationStation::print() const {
  printf("Load/Store Unit reservation station:\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}

void LoadStoreUnitReservationStation::getCurrentInstructions(Instruction* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void LoadStoreUnitReservationStation::getCurrentReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void LoadStoreUnitReservationStation::setNextInstruction(const Instruction instruction) {
  nextInstruction = instruction;
}

void LoadStoreUnitReservationStation::setNextReorderBufferIndex(const int index) {
  nextReorderBufferIndex = index;
}

int LoadStoreUnitReservationStation::readyToDispatch(const int index) const {
  Instruction instruction = instructions[index];
  //check that the source register are ready to use
  switch(instruction.opcode) {
    case NOOP:
      return 0;
    case LW:
      if(!loadStoreUnit->waitingForStore()) {
        return 1;
      }
    break;
    case SW:
      if(registerFile->getScoreBoardValue(operands[0]) && reorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return 1;
      }
      break;
    case LWR:
      //If the source registers are ready then continue
      if(!loadStoreUnit->waitingForStore() && registerFile->getScoreBoardValue(operands[1])) {
        return 1;
      }
      break;
    case SWR:
      //If the source registers are ready then continue
      if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1]) && reorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return 1;
      }
      break;
  }
  return 0;
}

//dispatch bound fetch
void LoadStoreUnitReservationStation::dispatch(const int index) {
  Instruction instruction = instructions[index];
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