//===========================================
//included header file containing interface
#include "fetch_unit.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "decode_issue_unit.h"

//===========================================
//class implementation

FetchUnit::FetchUnit(Instructions instructions, int* pc, DecodeIssueUnit* decodeIssueUnit) :
  instructions(instructions),
  pc(pc),
  decodeIssueUnit(decodeIssueUnit),
  currentInstruction((Instruction) {0,0,0,0})
{}

void FetchUnit::execute() {
  if(*pc <= instructions.getNumOfInstructions()) {
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

void FetchUnit::print() {
  printf("FETCHED INSTRUCTION: ");
  printInstruction(currentInstruction);
}

void FetchUnit::pipe() {
  //put the fetched instruction into the instruction register
  decodeIssueUnit->setNextInstruction(currentInstruction);
}

void FetchUnit::flush() {
  currentInstruction = (Instruction) {0,0,0,0};
}