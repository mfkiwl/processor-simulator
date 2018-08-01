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

ALUReservationStation::ALUReservationStation(RegisterFile* const registerFile, ALU* const alu, const int size) : 
  registerFile(registerFile),
  alu(alu),
  size(size),
  instructions(new Instruction[size]),
  reorderBufferIndexes(new int[size]),
  numReservedSpaces(0),
  nextInstructions(new Instruction[size]),
  nextReorderBufferIndexes(new int[size]),
  opcode(0),
  operands(new int[3]),
  reorderBufferIndex(-1),
  dispatchIndex(-1)
{
  //inialise arrays
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
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
  addNextInstructions();
  //clear the nextInstructions and nextReorderBufferIndexes
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
  }
  //reset the number of reserved spaces
  numReservedSpaces = 0;
}

//reset all member variables
void ALUReservationStation::flush() {
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
  }
  numReservedSpaces = 0;
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
  reorderBufferIndex = -1;
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

bool ALUReservationStation::freeSpace() const {
  int count = 0;
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      count++;
    }
  }
  count += numReservedSpaces;
  if(count == size) {
    return false;
  }
  else {
    return true;
  }
}

void ALUReservationStation::reserveSpace() {
  numReservedSpaces++;
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

void ALUReservationStation::addNextInstructions() {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode != NOOP) {
      int index = findFreePosition();
      instructions[index] = nextInstructions[i];
      reorderBufferIndexes[index] = nextReorderBufferIndexes[i];
    }
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

void ALUReservationStation::setNextInstruction(const Instruction instruction, const int rbi) {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode == NOOP) {
      nextInstructions[i] = instruction;
      nextReorderBufferIndexes[i] = rbi;
      break;
    }
  }
}