//===========================================
//included header file containing interface
#include "reorder_buffer.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "register_file.h"
#include "memory.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

using namespace std;

ReorderBuffer::ReorderBuffer(RegisterFile* const registerFile, Memory* const memory, int* const pc, 
int* const runningFlag, int* const noOfInstructionsExecuted, const int bufferSize, const int numFields, 
const int issueWindowSize) : 
  registerFile(registerFile),
  memory(memory),
  pc(pc),
  flushFlag(false),
  runningFlag(runningFlag),
  bufferSize(bufferSize),
  buffer(new int*[bufferSize]),
  head(0),
  tail(0),
  numFields(numFields),
  instructions(new Instruction[bufferSize]),
  noOfInstructionsExecuted(noOfInstructionsExecuted),
  issueWindowSize(issueWindowSize)
{
  //dynamically allocated a 2d array to the read and write buffer
  for(int i = 0; i < bufferSize; i++) {
    buffer[i] = new int[numFields];
  }
  //initialise all elements of the read and write buffer to zero
  for(int i = 0; i < bufferSize; i++) {
    for(int j = 0; j < numFields; j++) {
      buffer[i][j] = -1;
    }
  }
  //initialise all of the instructions
  for(int i = 0; i < bufferSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
  }
}

bool ReorderBuffer::freeSpace() const {
  if(((head + 1) % bufferSize) != tail) {
    return true;
  }
  else {
    return false;
  }
}

bool ReorderBuffer::empty() const {
  if(tail == head && instructions[head].opcode == NOOP) {
    return true;
  }
  else {
    return false;
  }
}

int ReorderBuffer::addEntry(const Type type, const int branchTargetAddress, const int architecturalRegister,
const int physicalRegister, const int previousPhysicalRegister, const Instruction instruction) {
  if(!empty()) {
    head = (head + 1) % bufferSize;
  }
  buffer[head][TYPE] = type;
  buffer[head][STATUS] = ISSUED;
  buffer[head][RESULT] = 0;
  buffer[head][BRANCH_TARGET_ADDRESS] = branchTargetAddress;
  buffer[head][ARCHITECTURAL_REGISTER] = architecturalRegister;
  buffer[head][PHYSICAL_REGISTER] = physicalRegister;
  buffer[head][PREVIOUS_PHYSICAL_REGISTER] = previousPhysicalRegister;
  instructions[head] = instruction;
  return head;
}

//commiting an entry when it can each cycle
void ReorderBuffer::execute() {
  for(int i = 0; i < issueWindowSize; i++) {
    if(buffer[tail][STATUS] == FINISHED) {
      //buffer[tail][STATUS] = -1;
      if(buffer[tail][TYPE] == STORE_TO_REGISTER) {
        //write the result to the register
        registerFile->setPhysicalRegisterValue(buffer[tail][PHYSICAL_REGISTER], buffer[tail][RESULT]);
        //update the rollback rename table
        registerFile->setRollbackRenameTableMapping(buffer[tail][ARCHITECTURAL_REGISTER], buffer[tail][PHYSICAL_REGISTER]);
        //Set the scoreBoard of the destination register to 1
        registerFile->setScoreBoardValue(buffer[tail][PHYSICAL_REGISTER], 1);
        //free the previous physical register
        registerFile->freePhysicalRegister(buffer[tail][PREVIOUS_PHYSICAL_REGISTER]);
      }
      if(buffer[tail][TYPE] == STORE_TO_MEMORY) {

      }
      if(buffer[tail][TYPE] == JUMP && buffer[tail][RESULT]) {
        *pc = buffer[tail][BRANCH_TARGET_ADDRESS];
        flushFlag = true;
        break;
      }
      if(buffer[tail][TYPE] == SYSCALL) {
        *runningFlag = 0;
      }
      //reset the reorder buffer entry
      resetEntry(tail);
      //increment the number of instructions that we have executed
      (*noOfInstructionsExecuted)++;
      //increment the tail position
      tail = (tail + 1) % bufferSize;
    }
  }
}

void ReorderBuffer::resetEntry(const int index) {
  instructions[index] = (Instruction) {0,0,0,0};
  for(int i = 0; i < numFields; i++) {
    buffer[index][i] = -1;
  }
}

void ReorderBuffer::executingEntry(const int i) {
  buffer[i][STATUS] = EXECUTING;
}

void ReorderBuffer::finishedEntry(const int i, const int result) {
  buffer[i][STATUS] = FINISHED;
  buffer[i][RESULT] = result;
}

void ReorderBuffer::writeResult(const int i, const int r) {
  buffer[i][RESULT] = r;
}


void ReorderBuffer::flush() {
  for(int i = 0; i < bufferSize; i++) {
    buffer[i][TYPE] = -1;
    buffer[i][STATUS] = -1;
    buffer[i][RESULT] = -1;
    buffer[i][BRANCH_TARGET_ADDRESS] = -1;
    buffer[i][PHYSICAL_REGISTER] = -1;
    buffer[i][PREVIOUS_PHYSICAL_REGISTER] = -1;
  }
  for(int i = 0; i < bufferSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
  }
  head = 0;
  tail = 0;
  flushFlag = false;
}

void ReorderBuffer::printTail() const {
  printf("REORDER BUFFER TAIL:");
  printInstruction(instructions[tail]);
}

void ReorderBuffer::print() const {
  printf("Reorder Buffer:\n");
  for(int i = tail; i < head; i++) {
    if(buffer[i][STATUS] == FINISHED) {
      printf("FINISHED : ");
    }
    printInstruction(instructions[i]);
  }
}

int ReorderBuffer::getTailIndex() const {
  return tail;
}

int ReorderBuffer::getHeadIndex() const {
  return head;
}

void ReorderBuffer::getReorderBufferInstructions(Instruction* const copy) const {
  for(int i = 0; i < bufferSize; i++) {
    copy[i] = instructions[i];
  }
}

void ReorderBuffer::getReorderBufferFields(int** const copy) const {
  for(int i = 0; i < bufferSize; i++) {
    for(int j = 0; j < numFields; j++) {
      copy[i][j] = buffer[i][j];
    }
  }
}

bool ReorderBuffer::getFlushFlag() const {
  return flushFlag;
}