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

FetchUnit::FetchUnit(const Instructions instructionBuffer, int* const pc, DecodeIssueUnit* const decodeIssueUnit, 
  const int issueWindowSize, const bool branchPrediction) :
  instructionBuffer(instructionBuffer),
  pc(pc),
  decodeIssueUnit(decodeIssueUnit),
  issueWindowSize(issueWindowSize),
  instructions(new Instruction[issueWindowSize]),
  branchAddresses(new int[issueWindowSize]),
  branchPrediction(branchPrediction)
{
  //initialise all instructions to NOOPs
  for(int i = 0; i < issueWindowSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    branchAddresses[i] = -1;
  }
}

void FetchUnit::execute() {
  int numToFetch = decodeIssueUnit->numFreeSpaces();
  if(numToFetch > 0) {
    if(*pc < instructionBuffer.getNumOfInstructions()) {
      //fetch the next instructions
      fetchInstructions(numToFetch);
    }
  }
}

void FetchUnit::print() const {
  printf("FETCHED INSTRUCTION: ");
  printInstruction(instructions[0]);
}

void FetchUnit::pipe() {
  //put the fetched instruction into the instruction register
  decodeIssueUnit->setNextInstructions(instructions);
  decodeIssueUnit->setNextBranchAddresses(branchAddresses);
  for(int i = 0; i < issueWindowSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    branchAddresses[i] = -1;
  }
}

void FetchUnit::flush() {
  for(int i = 0; i < issueWindowSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
    branchAddresses[i] = -1;
  }
}

//============================================================================
//private functions

void FetchUnit::printBranchAddresses() {
  for(int i = 0; i < issueWindowSize; i++) {
    printf("%d ", branchAddresses[i]);
  }
}

void FetchUnit::fetchInstructions(int num) {
  for(int i = 0; i < num; i++) {
    if(*pc < instructionBuffer.getNumOfInstructions()) {
      instructions[i] = instructionBuffer.at(*pc);
      if(isABranchInstruction(instructions[i])) {
        if(branchPrediction) {
          branchAddresses[i] = *pc + 1;
          takeBranch(instructions[i]);
        }
        else {
          branchAddresses[i] = getBranchTargetAddress(instructions[i]);
          (*pc)++;
        }
      }
      else {
        (*pc)++;
      }
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

int FetchUnit::getBranchTargetAddress(Instruction instruction) const {
  switch(instruction.opcode) {
    case BEQ:
      return instruction.operands[2];
    case BNE:
      return instruction.operands[2];
    case BGEZ:
      return instruction.operands[1];
    case BGTZ:
      return instruction.operands[1];
    case BLEZ:
      return instruction.operands[1];
    case BLTZ:
      return instruction.operands[1];
    case J:
      return instruction.operands[0];
    case JR:
      return -1;
  }
  return -1;
}

//======================================
// getter and setter functions

void FetchUnit::getInstructions(Instruction copy[]) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = instructions[i];
  }
}