//===========================================
//included header file containing interface
#include "branch_unit_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <stdlib.h>

#include "register_file.h"
#include "branch_unit.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

//================================================================================================
//public functions

BranchUnitReservationStation::BranchUnitReservationStation(RegisterFile* const registerFile, 
  BranchUnit* const branchUnit, const int size) : 
  registerFile(registerFile),
  branchUnit(branchUnit),
  size(size),
  instructions(new Instruction[size]),
  reorderBufferIndexes(new int[size]),
  numReservedSpaces(0),
  nextInstructions(new Instruction[size]),
  nextReorderBufferIndexes(new int[size]),
  dispatchIndex(-1)
{
  //set all instructions to NOOPs
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
  }
}

void BranchUnitReservationStation::execute() {
  //try and find an instruction that can be dispatched
  int start = rand() % size;
  int i = start;
  do {
    if(readyToDispatch(i)) {
      dispatchIndex = i;
      break;
    }
    i = (i + 1) % size;
  }
  while(i != start);
}

void BranchUnitReservationStation::pipe() {
  //send current instruction to the branch unit
  if(dispatchIndex != -1) {

    //fetch the operand values that we have been waiting for
    fetchOperands(dispatchIndex);

    //send the decoded instruction to the execution unit
    branchUnit->setNextOpcode(instructions[dispatchIndex].opcode);
    branchUnit->setNextOperands(instructions[dispatchIndex].operands);
    //Send the reorder buffer index to the execution unit
    branchUnit->setNextReorderBufferIndex(reorderBufferIndexes[dispatchIndex]);
  
    //clear the dispatched instruction from the reservation station
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
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
  //reset the number of reserved spaces
  numReservedSpaces = 0;
}

void BranchUnitReservationStation::flush() {
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
}

void BranchUnitReservationStation::print() const {
  printf("Branch Unit reservation station::\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}


bool BranchUnitReservationStation::freeSpace() const {
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

void BranchUnitReservationStation::reserveSpace() {
  numReservedSpaces++;
}

//==========================================================================================
//private functions

int BranchUnitReservationStation::findFreePosition() const {
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode == NOOP) {
      return i;
    }
  }
  return -1;
}

void BranchUnitReservationStation::addNextInstructions() {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode != NOOP) {
      int index = findFreePosition();
      instructions[index] = nextInstructions[i];
      reorderBufferIndexes[index] = nextReorderBufferIndexes[i];
    }
  }
}

bool BranchUnitReservationStation::readyToDispatch(const int index) const {
  Instruction instruction = instructions[index];
  //check that the source register are ready to use
  switch(instruction.opcode) {
    case NOOP:
      return false;
    case BEQ:
    case BNE:
      if(registerFile->getScoreBoardValue(instruction.operands[0]) && registerFile->getScoreBoardValue(instruction.operands[1])) {
        return true;
      }
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      if(registerFile->getScoreBoardValue(instruction.operands[0])) {
        return true;
      }
      break;
    case J:
    case JR:
    case HALT:
      return true;
      break;
  }
  return false;
}

void BranchUnitReservationStation::fetchOperands(const int index) {
  //fetching the operands for the instruction
  switch(instructions[index].opcode) {
    case NOOP:
      break;
    case BEQ:
    case BNE:
      instructions[index].operands[0] = registerFile->getPhysicalRegisterValue(instructions[index].operands[0]);
      instructions[index].operands[1] = registerFile->getPhysicalRegisterValue(instructions[index].operands[1]);
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      instructions[index].operands[0] = registerFile->getPhysicalRegisterValue(instructions[index].operands[0]);
      break;
    case J:
    case JR:
    case HALT:
      break;
  }
}


//=====================================================================================
//getters and setters

void BranchUnitReservationStation::getCurrentInstructions(Instruction* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void BranchUnitReservationStation::getCurrentReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void BranchUnitReservationStation::setNextInstruction(const Instruction instruction, const int rbi) {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode == NOOP) {
      nextInstructions[i] = instruction;
      nextReorderBufferIndexes[i] = rbi;
      break;
    }
  }
}