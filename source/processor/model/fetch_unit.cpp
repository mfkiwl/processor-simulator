//===========================================
//included header file containing interface
#include "fetch_unit.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "decode_issue_unit.h"
#include "constants.h"

//===========================================
//class implementation

//============================================================
//public functions

FetchUnit::FetchUnit(const Instructions instructions, int* const pc, DecodeIssueUnit* const decodeIssueUnit, 
  const int issueWindowSize) :
  instructions(instructions),
  pc(pc),
  decodeIssueUnit(decodeIssueUnit),
  issueWindowSize(issueWindowSize),
  currentInstructions(new Instruction[issueWindowSize]),
  size(20),
  head(0),
  tail(0),
  branchAddresses(new int[size])
{
  //initialise all instructions to NOOPs
  for(int i = 0; i < issueWindowSize; i++) {
    currentInstructions[i] = (Instruction) {0,0,0,0};
  }
  for(int i = 0; i < size; i++) {
    branchAddresses[i] = -1;
  }
}

void FetchUnit::execute() {
  int numToFetch = decodeIssueUnit->numFreeSpaces();
  if(numToFetch > 0) {
    if(*pc < instructions.getNumOfInstructions()) {
      //fetch the next instructions
      fetchInstructions(numToFetch);
    }
  }
  else {
    for(int i = 0; i < issueWindowSize; i++) {
      currentInstructions[i] = (Instruction) {0,0,0,0};
    }
  }
}

void FetchUnit::print() const {
  printf("FETCHED INSTRUCTION: ");
  printInstruction(currentInstructions[0]);
}

void FetchUnit::pipe() {
  //put the fetched instruction into the instruction register
  decodeIssueUnit->setNextInstructions(currentInstructions);
  for(int i = 0; i < issueWindowSize; i++) {
    currentInstructions[i] = (Instruction) {0,0,0,0};  
  }
}

void FetchUnit::flush() {
  currentInstructions[0] = (Instruction) {0,0,0,0};
}

int FetchUnit::getTail() {
  int address = branchAddresses[tail];
  branchAddresses[tail] = -1;
  if(tail != head) {
    tail = (tail + 1) % size;
  }
  return address;
}

//============================================================================
//private functions

void FetchUnit::fetchInstructions(int num) {
  for(int i = 0; i < num; i++) {
    if(*pc < instructions.getNumOfInstructions()) {
      currentInstructions[i] = instructions.at(*pc);
      if(isABranchInstruction(currentInstructions[i])) {
        storeBranchAddress();
        takeBranch(currentInstructions[i]);
      }
      else {
        (*pc)++;
      }
    }
    else {
      currentInstructions[i] = (Instruction) {0,0,0,0};
    }
  }
}

bool FetchUnit::isABranchInstruction(Instruction instruction) const {
  switch(instruction.opcode) {
    case BEQ:
      return true;
    case BNE:
      return true;
    case BGEZ:
      return true;
    case BGTZ:
      return true;
    case BLEZ:
      return true;
    case BLTZ:
      return true;
    case J:
      return true;
    case JR:
      return true;
  }
  return false;
}

bool FetchUnit::takeBranch(Instruction instruction) {
  switch(instruction.opcode) {
    case BEQ:
      *pc = instruction.operands[2];
      return true;
    case BNE:
      *pc = instruction.operands[2];
      return true;
    case BGEZ:
      *pc = instruction.operands[1];
      return true;
    case BGTZ:
      *pc = instruction.operands[1];
      return true;
    case BLEZ:
      *pc = instruction.operands[1];
      return true;
    case BLTZ:
      *pc = instruction.operands[1];
      return true;
    case J:
      *pc = instruction.operands[0];
      return true;
    case JR:
      return true;
  }
  return false;
}

bool FetchUnit::empty() const {
  if(head == tail && branchAddresses[head] == -1) {
    return true;
  }
  else {
    return false;
  }
}

void FetchUnit::storeBranchAddress() {
  if(!empty()) {
    head = (head + 1) % size;
  }
  branchAddresses[head] = *pc;
}

void FetchUnit::printBranchAddresses() const {
  printf("head: %d\n", head);
  printf("tail: %d\n", tail);
  if(!empty()) {
    int i = tail;
    while(i != head) {
      printf("%d ", branchAddresses[i]);
      i = (i + 1) % size;
    }
    printf("%d ", branchAddresses[i]);
  }
}

//======================================
// getter and setter functions

Instruction FetchUnit::getCurrentInstruction() const {
  return currentInstructions[0];
}

void FetchUnit::getCurrentInstructions(Instruction* const copy) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = currentInstructions[i];
  }
}