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
  successful(false)
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
          successful = true;
        }
        break;
      case BNE:
        if(currentOperands[0] != currentOperands[1]) {
          successful = true;
        }
        break;
      case J:
        successful = true;
        break;
      case HALT:
        successful = true;
        break;
      default:
        successful = false;
        break;
    }
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(currentReorderBufferIndex, successful);
  }
}

void BranchUnit::pipe() {
  //send the current values
  if(currentOpcode != NOOP) {
    //reset the successful flag
    successful = false;
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
  nextOpcode = 0;
  currentOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    currentOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  currentReorderBufferIndex = -1;
  successful = 0;
}

bool BranchUnit::getSuccessful() const {
  return successful;
}

int BranchUnit::getReorderBufferIndex() const {
  return currentReorderBufferIndex;
}