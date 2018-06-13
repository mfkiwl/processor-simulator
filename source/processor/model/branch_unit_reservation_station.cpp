//===========================================
//included header file containing interface
#include "branch_unit_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "register_file.h"
#include "branch_unit.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

BranchUnitReservationStation::BranchUnitReservationStation(RegisterFile* const registerFile, BranchUnit* const branchUnit, const int size) : 
  registerFile(registerFile),
  branchUnit(branchUnit),
  nextInstruction((Instruction) {0,0,0,0}),
  nextReorderBufferIndex(-1),
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
  //zero out operands
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
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

void BranchUnitReservationStation::addInstruction(const Instruction instruction, const int rbi) {
  if(instruction.opcode != NOOP) {
    int index = findFreePosition();
    instructions[index] = instruction;
    reorderBufferIndexes[index] = rbi;
  }
}

int BranchUnitReservationStation::readyToDispatch(const Instruction instruction) const {
  //check that the source register are ready to use
  switch(instruction.opcode) {
    case NOOP:
      return 0;
    case BEQ:
    case BNE:
      if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1])) {
        return 1;
      }
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      if(registerFile->getScoreBoardValue(operands[0])) {
        return 1;
      }
      break;
    case HALT:
    case J:
    case JR:
      return 1;
      break;
  }
  return 0;
}

void BranchUnitReservationStation::getOperands(const Instruction instruction) {
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
    case BEQ:
    case BNE:
      registerNum = operands[0];
      val = registerFile->getRegisterValue(registerNum);
      operands[0] = val;
      registerNum = operands[1];
      val = registerFile->getRegisterValue(registerNum);
      operands[1] = val;
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      registerNum = operands[0];
      val = registerFile->getRegisterValue(registerNum);
      operands[0] = val;
      break;
    case J:
    case JR:
    case HALT:
      break;
  }
}

void BranchUnitReservationStation::execute() {
  //try and find an instruction that can be dispatched
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      if(readyToDispatch(instructions[i])) {
        getOperands(instructions[i]);
        reorderBufferIndex = reorderBufferIndexes[i];
        dispatchIndex = i;
        break;
      }
    }
  }
}

bool BranchUnitReservationStation::spaceInBuffer() const {
  return findFreePosition() != -1;
}

void BranchUnitReservationStation::pipe() {
  //send current instruction to the branch unit
  if(reorderBufferIndex != -1) {
  
    //clear the dispatched instruction from the reservation station
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[dispatchIndex] = -1;
    dispatchIndex = -1;

    //send the decoded instruction to the execution unit
    branchUnit->setNextOpcode(opcode);
    branchUnit->setNextOperands(operands);
    //Send the reorder buffer index to the execution unit
    branchUnit->setNextReorderBufferIndex(reorderBufferIndex);
        
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

void BranchUnitReservationStation::flush() {
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

void BranchUnitReservationStation::print() const {
  printf("Branch Unit reservation station::\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}

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

void BranchUnitReservationStation::setNextInstruction(const Instruction instruction) {
  nextInstruction = instruction;
}

void BranchUnitReservationStation::setNextReorderBufferIndex(const int index) {
  nextReorderBufferIndex = index;
}