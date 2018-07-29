//===========================================
//included header file containing interface
#include "alu_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "register_file.h"
#include "alu.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

using namespace std;

//==========================================================================
//public functions

ALUReservationStation::ALUReservationStation(RegisterFile* const registerFile, ALU* const alu, const int size, const int issueWindowSize) : 
  registerFile(registerFile),
  alu(alu),
  size(size),
  instructions(new Instruction[size]),
  reorderBufferIndexes(new int[size]),
  issueWindowSize(issueWindowSize),
  nextInstructions(new Instruction[issueWindowSize]),
  nextReorderBufferIndex(-1),
  opcode(0),
  operands(new int[3]),
  reorderBufferIndex(-1),
  dispatchIndex(-1)
{
  //inialise arrays
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
  }
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
  }
  //zero out operands
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
}

void ALUReservationStation::execute() {
  //try and find an instruction that can be dispatched
  for(int i = 0; i < size; i++) {
    if(readyToDispatch(i)) {
      fetchOperands(i);
      reorderBufferIndex = reorderBufferIndexes[i];
      dispatchIndex = i;
      break;
    }
  }
}

void ALUReservationStation::pipe() {
  //send current instruction to the alu
  if(reorderBufferIndex != -1) {

    //clear the dispatched instruction from the reservation station
    instructions[dispatchIndex] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[dispatchIndex] = -1;
    dispatchIndex = -1;

    //send the decoded instruction to the execution unit
    alu->setNextOpcode(opcode);
    alu->setNextOperands(operands);
    //Send the reorder buffer index to the alu
    alu->setNextReorderBufferIndex(reorderBufferIndex);
        
    //reset the outputs
    opcode = 0;
    for(int i = 0; i < 3; i++) {
      operands[i] = 0;
    }
    reorderBufferIndex = -1;
  }
  //add the next instruction to the buffer
  addInstruction(nextInstructions[0], nextReorderBufferIndex);
  //clear the nextInstruction and nextReorderBufferIndex
  nextInstructions[0] = (Instruction) {0,0,0,0};
  nextReorderBufferIndex = -1;
}

//reset all member variables
void ALUReservationStation::flush() {
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

bool ALUReservationStation::freeSpace() const {
  return findFreePosition() != -1;
}

//print information about the current
void ALUReservationStation::print() const {
  printf("ALU reservation station:\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}

//==============================================================================================
//private functions

int ALUReservationStation::findFreePosition() const {
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode == NOOP) {
      return i;
    }
  }
  return -1;
}

void ALUReservationStation::addInstruction(const Instruction instruction, const int rbi) {
  if(instruction.opcode != NOOP) {
    int index = findFreePosition();
    instructions[index] = instruction;
    reorderBufferIndexes[index] = rbi;
  }
}

bool ALUReservationStation::readyToDispatch(const int index) const {
  Instruction instruction = instructions[index];
  //check that the source registers are ready to use
  switch(instruction.opcode) {
    case NOOP:
      return 0;
      break;
    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:
      if(registerFile->getScoreBoardValue(instruction.operands[1]) && registerFile->getScoreBoardValue(instruction.operands[2])) {
        return 1;
      }
      break;
    case ADDI:
      if(registerFile->getScoreBoardValue(instruction.operands[1])) {
        return 1;
      }
      break;
    default:
      return 0;
      break;
    }
  return 0;
}

void ALUReservationStation::fetchOperands(const int index) {
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
    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:
      operands[1] = registerFile->getPhysicalRegisterValue(operands[1]);
      operands[2] = registerFile->getPhysicalRegisterValue(operands[2]);
      break;
    case ADDI:
      operands[1] = registerFile->getPhysicalRegisterValue(operands[1]);
      break;
  }
}

//=========================================================================================
//getters and setters

void ALUReservationStation::getCurrentInstructions(Instruction* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void ALUReservationStation::getCurrentReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void ALUReservationStation::setNextInstruction(const Instruction instruction) {
  nextInstructions[0] = instruction;
}

void ALUReservationStation::setNextReorderBufferIndex(const int index) {
  nextReorderBufferIndex = index;
}