//===========================================
//included header file containing interface
#include "branch_unit.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "reorder_buffer.h"
#include "constants.h"

//================================================================================================
//public functions

BranchUnit::BranchUnit(ReorderBuffer* const reorderBuffer) : 
  reorderBuffer(reorderBuffer),
  nextOpcode(0),
  nextReorderBufferIndex(-1),
  opcode(0),
  reorderBufferIndex(-1),
  successful(false)
{
  //set all operands to zero
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
}

void BranchUnit::execute() {
  if(opcode != NOOP) {
    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(reorderBufferIndex);
    //execute the instruction
    switch(opcode) {
      case BEQ:
        if(operands[0] == operands[1]) {
          successful = true;
        }
        break;
      case BNE:
        if(operands[0] != operands[1]) {
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
    reorderBuffer->finishedEntry(reorderBufferIndex, successful);
  }
}

void BranchUnit::pipe() {
  //send the  values
  if(opcode != NOOP) {
    //reset the successful flag
    successful = false;
  }

  //set the  values equal to the next values
  opcode = nextOpcode;
  for(int i = 0; i < 3; i++) {
    operands[i] = nextOperands[i];
  }
  reorderBufferIndex = nextReorderBufferIndex;

  //clear the next values
  nextOpcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

void BranchUnit::flush() {
  nextOpcode = 0;
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  reorderBufferIndex = -1;
  successful = 0;
}

//======================================================================================================
//getters and setters

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


bool BranchUnit::getSuccessful() const {
  return successful;
}

int BranchUnit::getReorderBufferIndex() const {
  return reorderBufferIndex;
}