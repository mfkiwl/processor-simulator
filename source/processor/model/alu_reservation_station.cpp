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
  nextInstructions(new Instruction[size]),
  nextOperandTypes(new OperandType*[size]),
  nextReorderBufferIndexes(new int[size]),
  instructions(new Instruction[size]),
  operandTypes(new OperandType*[size]),
  validBits(new bool*[size]),
  reorderBufferIndexes(new int[size]),
  numReservedSpaces(0),
  dispatchIndexes(new int[numALUs])
{
  //inialise arrays
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextOperandTypes[i] = new OperandType[3];
    for(int j = 0; j < 3; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    operandTypes[i] = new OperandType[3];
    for(int j = 0; j < 3; j++) {
      operandTypes[i][j] = NONE;
    }
    validBits[i] = new bool[3];
    for(int j = 0; j < 3; j++) {
      validBits[i][j] = false;
    }
    reorderBufferIndexes[i] = -1;
  }

  for(int i = 0; i < numALUs; i++) {
    dispatchIndexes[i] = -1;
  }
}

void ALUReservationStation::execute() {
  //try and fetch operands values that we are waiting for
  checkOperandAvailability();
  //try and find instructions that can be dispatched
  int count = 0;
  int start = rand() % size;
  int i = start;
  do {
    if(readyToDispatch(i)) {
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
      for(int j = 0; j < 3; j++) {
        operandTypes[i][j] = NONE;
      }
      for(int j = 0; j < 3; j++) {
        validBits[dispatchIndexes[i]][j] = false;
      }
      reorderBufferIndexes[dispatchIndexes[i]] = -1;
      dispatchIndexes[i] = -1;
    }
  }
  //add the next instruction to the buffer
  addNextInstructions();
  //clear the nextInstructions and nextReorderBufferIndexes
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < 3; j++) {
      nextOperandTypes[i][j] = NONE;
    }
    nextReorderBufferIndexes[i] = -1;
  }
  //reset the number of reserved spaces
  numReservedSpaces = 0;
}

//reset all member variables
void ALUReservationStation::flush() {
  for(int i = 0; i < size; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextReorderBufferIndexes[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < 3; j++) {
      operandTypes[i][j] = NONE;
    }
    for(int j = 0; j < 3; j++) {
      validBits[i][j] = false;
    }
    reorderBufferIndexes[i] = -1;
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

void ALUReservationStation::broadcast(int physicalRegister, int value) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case ADD:
      case AND:
      case MULT:
      case OR:
      case SUB:
        if(!validBits[i][1] && instructions[i].operands[1] == physicalRegister) {
          instructions[i].operands[1] = value;
          validBits[i][1] = true;
        }
        if(!validBits[i][2] && instructions[i].operands[2] == physicalRegister) {
          instructions[i].operands[2] = value;
          validBits[i][2] = true;
        }
        break;
      case ADDI:
        if(!validBits[i][1] && instructions[i].operands[1] == physicalRegister) {
          instructions[i].operands[1] = value;
          validBits[i][1] = true;
        }
        break;
    }
  }
}

//==============================================================================================
//private functions

void ALUReservationStation::checkOperandAvailability() {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case ADD:
      case AND:
      case MULT:
      case OR:
      case SUB:
        if(!validBits[i][1]) {
          if(registerFile->getScoreBoardValue(instructions[i].operands[1])) {
            instructions[i].operands[1] = registerFile->getPhysicalRegisterValue(instructions[i].operands[1]);
            validBits[i][1] = true;
            operandTypes[i][1] = CONSTANT;
          }
        }
        if(!validBits[i][2]) {
          if(registerFile->getScoreBoardValue(instructions[i].operands[2])) {
            instructions[i].operands[2] = registerFile->getPhysicalRegisterValue(instructions[i].operands[2]);
            validBits[i][2] = true;
            operandTypes[i][2] = CONSTANT;
          }
        }
        break;
      case ADDI:
        if(!validBits[i][1]) {
          if(registerFile->getScoreBoardValue(instructions[i].operands[1])) {
            instructions[i].operands[1] = registerFile->getPhysicalRegisterValue(instructions[i].operands[1]);
            validBits[i][1] = true;
            operandTypes[i][1] = CONSTANT;
          }
        }
        if(!validBits[i][2]) {
          validBits[i][2] = true;
          operandTypes[i][2] = CONSTANT;
        }
        break;
    }
  }
}

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
      for(int j = 0; j < 3; j++) {
        operandTypes[index][j] = nextOperandTypes[i][j];
      }
      reorderBufferIndexes[index] = nextReorderBufferIndexes[i];
    }
  }
}

bool ALUReservationStation::readyToDispatch(const int index) const {
  //check that all operands are valid
  switch(instructions[index].opcode) {
    case NOOP:
      return false;
      break;
    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:
      if(validBits[index][1] && validBits[index][2]) {
        return true;
      }
      break;
    case ADDI:
      if(validBits[index][1]) {
        return true;
      }
      break;
    default:
      return false;
      break;
    }
  return false;
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

void ALUReservationStation::setNextInstruction(const Instruction instruction, const OperandType types[], const int rbi) {
  for(int i = 0; i < size; i++) {
    if(nextInstructions[i].opcode == NOOP) {
      nextInstructions[i] = instruction;
      for(int j = 0; j < 3; j++) {
        nextOperandTypes[i][j] = types[j];
      }
      nextReorderBufferIndexes[i] = rbi;
      break;
    }
  }
}

void ALUReservationStation::getValidBits(bool** const copy) const {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < 3; j++) {
      copy[i][j] = validBits[i][j];
    }
  }
}