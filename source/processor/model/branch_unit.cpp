//===========================================
//included header file containing interface
#include "branch_unit.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "reorder_buffer.h"
#include "constants.h"

//===========================================
//class implementation

BranchUnit::BranchUnit(ReorderBuffer* const reorderBuffer) : 
  reorderBuffer(reorderBuffer),
  nextOpcode(0),
  currentOpcode(0),
  nextReorderBufferIndex(-1),
  currentReorderBufferIndex(-1),
  successful(0)
{
  //set all operands to zero
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    currentOperands[i] = 0;
  }
}

void BranchUnit::execute() {
  if(currentOpcode != NOOP) {
    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(currentReorderBufferIndex);
    //execute the instruction
    switch(currentOpcode) {
      case BEQ:
        if(currentOperands[0] == currentOperands[1]) {
          successful = 1;
        }
        break;
      case BNE:
        if(currentOperands[0] != currentOperands[1]) {
          successful = 1;
        }
        break;
      case J:
        successful = 1;
        break;
      case HALT:
        successful = 1;
        break;
      default:
        successful = 0;
        break;
    }
  }
}

void BranchUnit::pipe() {
  //send the current values
  if(currentOpcode != NOOP) {
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(currentReorderBufferIndex, successful);
    //reset the successful flag
    successful = 0;
  }

  //set the current values equal to the next values
  currentOpcode = nextOpcode;
  for(int i = 0; i < 3; i++) {
    currentOperands[i] = nextOperands[i];
  }
  currentReorderBufferIndex = nextReorderBufferIndex;

  //clear the next values
  nextOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

void BranchUnit::setNextOpcode(const int x) {
  nextOpcode = x;
}

void BranchUnit::setNextOperands(const int x[3]) {
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = x[i];
  }
}

void BranchUnit::setNextReorderBufferIndex(const int i) {
  nextReorderBufferIndex = i;
}

void BranchUnit::flush() {
  currentOpcode = 0;
  for(int i = 0; i < 3; i++) {
    currentOperands[i] = 0;
  }
  currentReorderBufferIndex = -1;
}