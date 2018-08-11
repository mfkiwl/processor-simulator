//===========================================
// included header file containing interface
#include "alu.h"

//===========================================
// included dependencies
#include "register_file.h"
#include "reorder_buffer.h"
#include "constants.h"

#include <stdio.h>

//===========================================
// class implementation

//===============================================================
//public functions

ALU::ALU() :
  reorderBuffer(NULL),
  nextOpcode(0),
  nextReorderBufferIndex(-1),
  opcode(0),
  reorderBufferIndex(-1),
  result(0)
{
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
}

ALU::ALU(ReorderBuffer* const reorderBuffer) : 
  reorderBuffer(reorderBuffer),
  nextOpcode(0),
  nextReorderBufferIndex(-1),
  opcode(0),
  reorderBufferIndex(-1),
  result(0)
{
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
}

void ALU::execute() {
  if(opcode != NOOP) {
    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(reorderBufferIndex);
    //execute the instruction
    switch(opcode) {
      case ADD: case ADDI:
        result = operands[1] + operands[2];
        break;
      case AND:
        result = operands[1] && operands[2];
        break;
      case MULT:
        result = operands[1] * operands[2];
        break;
      case OR:
        result = operands[1] || operands[2];
        break;
     case SUB:
        result = operands[1] - operands[2];
        break;
    }
    //tell the reorder buffer the result
    reorderBuffer->finishedEntry(reorderBufferIndex, result);
  }
}

void ALU::pipe() {
  
  //reset the  result to zero
  result = 0;

  //set the  values equal to the next values
  opcode = nextOpcode;
  for(int i = 0; i < 3; i++) {
    operands[i] = nextOperands[i]; 
  }
  reorderBufferIndex = nextReorderBufferIndex;

  //reset the next values
  nextOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

//========================================================================
//getters and setters

void ALU::setNextOpcode(const int x) {
  nextOpcode = x;
}

void ALU::setNextOperands(const int x[3]) {
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = x[i];
  }
}

void ALU::setNextReorderBufferIndex(const int i) {
  nextReorderBufferIndex = i;
}

void ALU::flush() {
  nextOpcode = 0;
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  reorderBufferIndex = -1;
  result = 0;
}

int ALU::getResult() const {
  return result;
}

int ALU::getReorderBufferIndex() const {
  return reorderBufferIndex;
}

void ALU::setReorderBufferPointer(ReorderBuffer* p) {
  reorderBuffer = p;
}