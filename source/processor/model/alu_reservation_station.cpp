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

ALUReservationStation::ALUReservationStation(RegisterFile* registerFile, ALU* alu, int size) : 
  registerFile(registerFile),
  alu(alu),
  size(size),
  opcode(0),
  reorderBufferIndex(-1)
{
  //allocate memory to the buffer
  instructions = new Instruction[size];
  //inialise all instructions to NOOPs
  for(int i = 0; i < size; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
  }
  //allocate memory to the reorder buffer indexes array
  reorderBufferIndexes = new int[size];
  //initialise all reorder buffer indexes to -1
  for(int i = 0; i < size; i++) {
    reorderBufferIndexes[i] = -1;
  }
  //allocate memory for the operands array
  operands = new int[3];
  //initialising operands
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
}

void ALUReservationStation::execute() {
  //try and find an instruction that can be dispatched
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      if(readyToDispatch(instructions[i])) {
        dispatch(instructions[i]);
        reorderBufferIndex = reorderBufferIndexes[i];

        //printf("DISPATCHING INSTRUCTION: ");
        //printInstruction(instructions[i]);

        //clear the dispatched instruction from the reservation station
        instructions[i] = (Instruction) {0,0,0,0};
        reorderBufferIndexes[i] = -1;
        break;
      }
    }
  }
}

void ALUReservationStation::addInstruction(Instruction instruction, int rbi) {
  //printf("ADDED INSTRUCTION: ");
  //printInstruction(instruction);
  int index = findFreePosition();
  instructions[index] = instruction;
  reorderBufferIndexes[index] = rbi;
}

int ALUReservationStation::findFreePosition() {
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode == NOOP) {
      return i;
    }
  }
  return -1;
}

void ALUReservationStation::pipe() {
  if(reorderBufferIndex != -1) {
    //send the decoded instruction to the execution unit
    alu->setOpcode(opcode);
    alu->setOperands(operands);
    //Send the reorder buffer index to the alu
    alu->setReorderBufferIndex(reorderBufferIndex);
        
    //reset the outputs
    opcode = 0;
    for(int i = 0; i < 3; i++) {
      operands[i] = 0;
    }
    reorderBufferIndex = -1;
  }
}

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

void ALUReservationStation::print() {
  printf("ALU reservation station:\n");
  for(int i = 0; i < size; i++) {
    if(instructions[i].opcode != NOOP) {
      printInstruction(instructions[i]);
    }
  }
}

int ALUReservationStation::readyToDispatch(Instruction instruction) {
  //check that the source registers are ready to use
  switch(instruction.opcode) {
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

void ALUReservationStation::dispatch(Instruction instruction) {
  //getting the opcode and incomplete operands from the instruction
  opcode = instruction.opcode;
  operands = new int[3];
  for(int i = 0; i < 3; i++) {
    operands[i] = instruction.operands[i];
  }
  //temp variables
  int registerNum;
  int val;
  //fetching the operands for the instruction
  switch(opcode) {
    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:
      for(int i = 1; i < 3; i++) {
        registerNum = operands[i];
        val = registerFile->getRegisterValue(registerNum);
        operands[i] = val;
      }
      break;
    case ADDI:
      registerNum = operands[1];
      val = registerFile->getRegisterValue(registerNum);
      operands[1] = val;
      break;
  }
}

void ALUReservationStation::getCurrentInstructions(Instruction* copy) {
  for(int i = 0; i < size; i++) {
    copy[i] = instructions[i];
  }
}