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

//======================================================================================================
//public functions

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
  reorderBufferIndex(-1),
  dispatchIndex(-1)
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
  //dispatch the instruction at the tail if is is ready, this results in all of the load and store
  //instruction being exeucted in order
  if(readyToDispatch(tail)) {
    fetchOperands(tail);
    reorderBufferIndex = reorderBufferIndexes[tail];
    dispatchIndex = tail;
    tail = (tail + 1) % size;
  }
}

void LoadStoreUnitReservationStation::addInstruction(const Instruction instruction, const int rbi) {
  if(instruction.opcode != NOOP) {
    instructions[head] = instruction;
    reorderBufferIndexes[head] = rbi;
    head = (head + 1) % size;
  }
}

bool LoadStoreUnitReservationStation::freeSpace() const {
  if(tail == head && instructions[head].opcode != NOOP) {
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
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[dispatchIndex] = -1;
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

//============================================================================================
//private functions

bool LoadStoreUnitReservationStation::readyToDispatch(const int index) const {
  Instruction instruction = instructions[index];
  //check that the source register are ready to use
  switch(instruction.opcode) {
    case NOOP:
      return false;
    case LW:
      //ready
      return true;
    break;
    case SW:
      //ready if the source registers are ready and the instruction is at the ROB tail
      if(registerFile->getScoreBoardValue(instruction.operands[0]) && reorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return true;
      }
      break;
    case LWR:
      //ready is the source registers are ready
      if(registerFile->getScoreBoardValue(instruction.operands[1])) {
        return true;
      }
      break;
    case SWR:
      //ready if the source registers are ready and the instruction is at the ROB tail
      if(registerFile->getScoreBoardValue(instruction.operands[0]) && registerFile->getScoreBoardValue(instruction.operands[1]) && reorderBufferIndexes[index] == reorderBuffer->getTailIndex()) {
        return true;
      }
      break;
  }
  return false;
}

//dispatch bound fetch
void LoadStoreUnitReservationStation::fetchOperands(const int index) {
  Instruction instruction = instructions[index];
  //getting the opcode and incomplete operands from the instruction
  opcode = instruction.opcode;
  for(int i = 0; i < 3; i++) {
    operands[i] = instruction.operands[i];
  }
  //fetching the operands for the instruction
  switch(opcode) {
    case NOOP:
      break;
    case LW:
      break;
    case SW:
      operands[0] = registerFile->getPhysicalRegisterValue(operands[0]);
      break;
    case LWR:
      operands[1] = registerFile->getPhysicalRegisterValue(operands[1]);
      break;
    case SWR:
      operands[0] = registerFile->getPhysicalRegisterValue(operands[0]);
      operands[1] = registerFile->getPhysicalRegisterValue(operands[1]);
      break;
  }
}

//=============================================================================================
//getters and setters

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