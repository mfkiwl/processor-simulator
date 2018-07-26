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
  numInstructions(4),
  currentInstructions(new Instruction[numInstructions])
{
  //initialise all instructions to NOOPs
  for(int i = 0; i < numInstructions; i++) {
    currentInstructions[i] = (Instruction) {0,0,0,0};
  }
}

void FetchUnit::fetchInstructions() {
  for(int i = 0; i < numInstructions; i++) {
    if(*pc + i < instructions.getNumOfInstructions()) {
      currentInstructions[i] = instructions.at(*pc + i);
    }
  }
}

void FetchUnit::execute(bool blocking) {
  if(!blocking && *pc < instructions.getNumOfInstructions()) {
    //fetch instructions
    fetchInstructions();
    //increment the program counter
    (*pc)++;
  }
  else {
    //next instruction is noop if pc exceeds number of instructions
    currentInstructions[0] = (Instruction) {0,0,0,0};
  }
}

void FetchUnit::print() const {
  printf("FETCHED INSTRUCTION: ");
  printInstruction(currentInstructions[0]);
}

void FetchUnit::pipe(bool blocking) {
  if(!blocking) {
    //put the fetched instruction into the instruction register
    decodeIssueUnit->setNextInstruction(currentInstructions[0]);
  }
}

void FetchUnit::flush() {
  currentInstructions[0] = (Instruction) {0,0,0,0};
}

//======================================
// getter functions

Instruction FetchUnit::getCurrentInstruction() const {
  return currentInstructions[0];
}