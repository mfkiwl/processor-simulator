//===========================================
//included header file containing interface
#include "branch_unit_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <stdlib.h>

#include "reorder_buffer.h"
#include "register_file.h"
#include "branch_unit.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

//================================================================================================
//public functions

BranchUnitReservationStation::BranchUnitReservationStation(ReorderBuffer* const reorderBuffer, 
  RegisterFile* const registerFile, BranchUnit* const branchUnit, const int size) :
  reorderBuffer(reorderBuffer),
  registerFile(registerFile),
  branchUnit(branchUnit),
  size(size),
  nextInstructions(new Instruction[size]),
  nextOperandTypes(new OperandType*[size]),
  nextReorderBufferIndexes(new int[size]),
  instructions(new Instruction[size]),
  operandTypes(new OperandType*[size]),
  reorderBufferIndexes(new int[size]),
  numReservedSpaces(0),
  dispatchIndex(-1)
{
  //set all instructions to NOOPs
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
    reorderBufferIndexes[i] = -1;
  }
}

void BranchUnitReservationStation::execute() {
  //try and fetch operands values that we are waiting for
  checkOperandAvailability();
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

    //send the decoded instruction to the execution unit
    branchUnit->setNextOpcode(instructions[dispatchIndex].opcode);
    branchUnit->setNextOperands(instructions[dispatchIndex].operands);
    //Send the reorder buffer index to the execution unit
    branchUnit->setNextReorderBufferIndex(reorderBufferIndexes[dispatchIndex]);
  
    //clear the dispatched instruction from the reservation station
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
    for(int j = 0; j < numOfOperands; j++) {
      operandTypes[dispatchIndex][j] = NONE;
    }
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
  //reset the number of reserved spaces
  numReservedSpaces = 0;
}

void BranchUnitReservationStation::flush() {
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < numOfOperands; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < numOfOperands; j++) {
      operandTypes[i][j] = NONE;
    }
    reorderBufferIndexes[i] = -1;
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

void BranchUnitReservationStation::broadcast(int physicalRegister, int value) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case BEQ:
      case BNE:
        if(!(operandTypes[i][0] == CONSTANT) && instructions[i].operands[0] == physicalRegister) {
          instructions[i].operands[0] = value;
          operandTypes[i][0] = CONSTANT;
        }
        if(!(operandTypes[i][1] == CONSTANT) && instructions[i].operands[1] == physicalRegister) {
          instructions[i].operands[1] = value;
          operandTypes[i][1] = CONSTANT;
        }
        break;
      case BGEZ:
      case BGTZ:
      case BLEZ:
      case BLTZ:
        break;
      case J:
      case JR:
      case HALT:
        break;
    }
  }
}



//==========================================================================================
//private functions

void BranchUnitReservationStation::checkOperandAvailability() {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case BEQ:
      case BNE:
        if(!(operandTypes[i][0] == CONSTANT)) {
          if(operandTypes[i][0] == REGISTER) {
            instructions[i].operands[0] = registerFile->getRegisterValue(instructions[i].operands[0]);
            operandTypes[i][0] = CONSTANT;
          }
          else if(operandTypes[i][0] == ROB) {
            if(reorderBuffer->isEntryFinished(instructions[i].operands[0])) {
              instructions[i].operands[0] = reorderBuffer->getEntryResult(instructions[i].operands[0]);
              operandTypes[i][0] = CONSTANT;
            }
          }
        }
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
      case BGEZ:
      case BGTZ:
      case BLEZ:
      case BLTZ:
        break;
      case J:
      case JR:
      case HALT:
        break;
    }
  }
}


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
      for(int j = 0; j < numOfOperands; j++) {
        operandTypes[index][j] = nextOperandTypes[i][j];
      }
      reorderBufferIndexes[index] = nextReorderBufferIndexes[i];
    }
  }
}

bool BranchUnitReservationStation::readyToDispatch(const int index) const {
  //check that operands are valid
  switch(instructions[index].opcode) {
    case NOOP:
      return false;
    case BEQ:
    case BNE:
      if(operandTypes[index][0] == CONSTANT && operandTypes[index][1] == CONSTANT) {
        return true;
      }
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      break;
    case J:
    case JR:
    case HALT:
      return true;
      break;
  }
  return false;
}

//=====================================================================================
//getters and setters

void BranchUnitReservationStation::setNextInstruction(const Instruction instruction, const OperandType types[], 
  const int rbi) 
{
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

void BranchUnitReservationStation::getCurrentInstructions(Instruction copy[]) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void BranchUnitReservationStation::getCurrentReorderBufferIndexes(int copy[]) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void BranchUnitReservationStation::getOperandTypes(OperandType copy[][numOfOperands]) const {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < numOfOperands; j++) {
      copy[i][j] = operandTypes[i][j];
    }
  }
}