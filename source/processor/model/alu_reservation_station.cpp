//===========================================
//included header file containing interface
#include "alu_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <stdlib.h>

#include "register_file.h"
#include "alu.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

using namespace std;

//==========================================================================
//public functions

ALUReservationStation::ALUReservationStation(RegisterFile* const registerFile, const int numALUs, ALU* const alu, 
  const int size) : 
  registerFile(registerFile),
  numALUs(numALUs),
  alu(alu),
  size(size),
  instructions(new Instruction[size]),
  reorderBufferIndexes(new int[size]),
  numReservedSpaces(0),
  nextInstructions(new Instruction[size]),
  nextReorderBufferIndexes(new int[size]),
  dispatchIndexes(new int[numALUs])
{
  //inialise arrays
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    reorderBufferIndexes[i] = -1;
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
  }
  for(int i = 0; i < numALUs; i++) {
    dispatchIndexes[i] = -1;
  }
}

void ALUReservationStation::execute() {
  int count = 0;
  //try and find an instruction that can be dispatched
  int start = rand() % size;
  int i = start;
  do {
    if(readyToDispatch(i)) {
      fetchOperands(i);
      dispatchIndexes[count] = i;
      count++;
      if(count == numALUs) {
        break;
      }
    }
    i = (i + 1) % size;
  }
  while(i != start);
}

void ALUReservationStation::pipe() {
  //send current instruction to the alu
  for(int i = 0; i < numALUs; i++) {
    if(dispatchIndexes[i] != -1) {

      //send the decoded instruction to the execution unit
      alu[i].setNextOpcode(instructions[dispatchIndexes[i]].opcode);
      alu[i].setNextOperands(instructions[dispatchIndexes[i]].operands);
      //Send the reorder buffer index to the alu
      alu[i].setNextReorderBufferIndex(reorderBufferIndexes[dispatchIndexes[i]]);

      //clear the dispatched instruction from the reservation station
      instructions[dispatchIndexes[i]] = (Instruction) {0,0,0,0};
      reorderBufferIndexes[dispatchIndexes[i]] = -1;
      dispatchIndexes[i] = -1;
    }
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
  for(int i = 0; i < numALUs; i++) {
    dispatchIndexes[i] = -1;
  }
  numReservedSpaces = 0;
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
  for(int i = 0; i < numALUs; i++) {
    if(dispatchIndexes[i] != -1) {
      count--;
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
  //fetching the operands for the instruction
  switch(instructions[index].opcode) {
    case NOOP:
      break;
    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:
      instructions[index].operands[1] = registerFile->getPhysicalRegisterValue(instructions[index].operands[1]);
      instructions[index].operands[2] = registerFile->getPhysicalRegisterValue(instructions[index].operands[2]);
      break;
    case ADDI:
      instructions[index].operands[1] = registerFile->getPhysicalRegisterValue(instructions[index].operands[1]);
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