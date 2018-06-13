//===========================================
//included header file containing interface
#include "fetch_unit.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "decode_issue_unit.h"

//===========================================
//class implementation

FetchUnit::FetchUnit(const Instructions instructions, int* const pc, DecodeIssueUnit* const decodeIssueUnit) :
  instructions(instructions),
  pc(pc),
  decodeIssueUnit(decodeIssueUnit),
  currentInstruction((Instruction) {0,0,0,0})
{}

void FetchUnit::execute(bool blocking) {
  if(!blocking && *pc <= instructions.getNumOfInstructions()) {
    //fetch the next instruction (-1 so that pc of 1 refers to the first instruction on line 1)
    currentInstruction = instructions.at(*pc - 1);
    //increment the program counter
    (*pc)++;
  }
  else {
    //next instruction is noop if pc exceeds number of instructions
    currentInstruction = (Instruction) {0,0,0,0};
  }
}

void FetchUnit::print() const {
  printf("FETCHED INSTRUCTION: ");
  printInstruction(currentInstruction);
}

void FetchUnit::pipe(bool blocking) {
  if(!blocking) {
    //put the fetched instruction into the instruction register
    decodeIssueUnit->setNextInstruction(currentInstruction);
  }
}

void FetchUnit::flush() {
  currentInstruction = (Instruction) {0,0,0,0};
}

//======================================
// getter functions

Instruction FetchUnit::getCurrentInstruction() const {
  return currentInstruction;
}