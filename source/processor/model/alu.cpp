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

ALU::ALU(ReorderBuffer* const reorderBuffer) : 
  reorderBuffer(reorderBuffer),
  nextOpcode(0),
  currentOpcode(0),
  nextReorderBufferIndex(-1),
  currentReorderBufferIndex(-1),
  currentResult(0)
{
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    currentOperands[i] = 0;
  }
}

void ALU::execute() {
  if(currentOpcode != NOOP) {
    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(currentReorderBufferIndex);
    //execute the instruction
    switch(currentOpcode) {
      case ADD: case ADDI:
        currentResult = currentOperands[1] + currentOperands[2];
        break;
      case AND:
        currentResult = currentOperands[1] && currentOperands[2];
        break;
      case MULT:
        currentResult = currentOperands[1] * currentOperands[2];
        break;
      case OR:
        currentResult = currentOperands[1] || currentOperands[2];
        break;
     case SUB:
        currentResult = currentOperands[1] - currentOperands[2];
        break;
    }
  }
}

void ALU::pipe() {
  //use the current values
  if(currentOpcode != NOOP) {
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(currentReorderBufferIndex, currentResult);
  }

  //set the current values equal to the next values
  currentOpcode = nextOpcode;
  for(int i = 0; i < 3; i++) {
    currentOperands[i] = nextOperands[i]; 
  }
  currentReorderBufferIndex = nextReorderBufferIndex;

  //reset the next values
  nextOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

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
  currentOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    currentOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  currentReorderBufferIndex = -1;
  currentResult = 0;
}

int ALU::getResult() const {
  return currentResult;
}

int ALU::getReorderBufferIndex() const {
  return currentReorderBufferIndex;
}