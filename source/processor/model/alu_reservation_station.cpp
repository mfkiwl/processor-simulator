//===========================================
//included header file containing interface
#include "alu_reservation_station.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <stdlib.h>

#include "reorder_buffer.h"
#include "register_file.h"
#include "alu.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

using namespace std;

//==========================================================================
//public functions

ALUReservationStation::ALUReservationStation(ReorderBuffer* const reorderBuffer, RegisterFile* const registerFile, 
  const int numALUs, ALU* const alu, const int size) :
  reorderBuffer(reorderBuffer),
  registerFile(registerFile),
  numALUs(numALUs),
  alu(alu),
  size(size),
  nextInstructions(new Instruction[size]),
  nextOperandTypes(new OperandType*[size]),
  nextReorderBufferIndexes(new int[size]),
  instructions(new Instruction[size]),
  operandTypes(new OperandType*[size]),
  reorderBufferIndexes(new int[size]),
  numReservedSpaces(0),
  dispatchIndexes(new int[numALUs])
{
  //inialise arrays
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
      for(int j = 0; j < numOfOperands; j++) {
        operandTypes[dispatchIndexes[i]][j] = NONE;
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
    for(int j = 0; j < numOfOperands; j++) {
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

void ALUReservationStation::broadcast(int robEntry, int value) {
  for(int i = 0; i < size; i++) {
    switch(instructions[i].opcode) {
      case NOOP:
        break;
      case ADD:
      case AND:
      case MULT:
      case OR:
      case SUB:
        if((operandTypes[i][1] == ROB) && instructions[i].operands[1] == robEntry) {
          instructions[i].operands[1] = value;
          operandTypes[i][1] = CONSTANT;
        }
        if((operandTypes[i][2] == ROB) && instructions[i].operands[2] == robEntry) {
          instructions[i].operands[2] = value;
          operandTypes[i][2] = CONSTANT;
        }
        break;
      case ADDI:
        if((operandTypes[i][1] == ROB) && instructions[i].operands[1] == robEntry) {
          instructions[i].operands[1] = value;
          operandTypes[i][1] = CONSTANT;
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
        if(!(operandTypes[i][2] == CONSTANT)) {
          if(operandTypes[i][2] == REGISTER) {
            instructions[i].operands[2] = registerFile->getRegisterValue(instructions[i].operands[2]);
            operandTypes[i][2] = CONSTANT;
          }
          else if(operandTypes[i][2] == ROB) {
            if(reorderBuffer->isEntryFinished(instructions[i].operands[2])) {
              instructions[i].operands[2] = reorderBuffer->getEntryResult(instructions[i].operands[2]);
              operandTypes[i][2] = CONSTANT;
            }
          }
        }
        break;
      case ADDI:
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
      for(int j = 0; j < numOfOperands; j++) {
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
      if(operandTypes[index][1] == CONSTANT && operandTypes[index][2] == CONSTANT) {
        return true;
      }
      break;
    case ADDI:
      if(operandTypes[index][1] == CONSTANT && operandTypes[index][2] == CONSTANT) {
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

void ALUReservationStation::setNextInstruction(const Instruction instruction, const OperandType types[], 
  const int rbi) {
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

void ALUReservationStation::getCurrentInstructions(Instruction copy[]) const {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}

void ALUReservationStation::getCurrentReorderBufferIndexes(int copy[]) const {
  for(int i = 0; i < size; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void ALUReservationStation::getOperandTypes(OperandType copy[][numOfOperands]) const {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < numOfOperands; j++) {
      copy[i][j] = operandTypes[i][j];
    }
  }
}