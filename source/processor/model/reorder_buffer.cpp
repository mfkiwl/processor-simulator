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

ReorderBuffer::ReorderBuffer(RegisterFile* const registerFile, Memory* const memory, int* const pc, int* const flushFlag, int* const runningFlag, int* const noOfInstructionsExecuted, const int bufferSize) : 
  registerFile(registerFile),
  memory(memory),
  pc(pc),
  flushFlag(flushFlag),
  runningFlag(runningFlag),
  bufferSize(bufferSize),
  buffer(new int*[bufferSize]),
  head(0),
  tail(0),
  bufferEntryFields(4),
  instructions(new Instruction[bufferSize]),
  noOfInstructionsExecuted(noOfInstructionsExecuted)
{
  //dynamically allocated a 2d array to the read and write buffer
  for(int i = 0; i < bufferSize; i++) {
    buffer[i] = new int[bufferEntryFields];
  }
  //initialise all elements of the read and write buffer to zero
  for(int i = 0; i < bufferSize; i++) {
    for(int j = 0; j < bufferEntryFields; j++) {
      buffer[i][j] = -1;
    }
  }
  //initialise all of the instructions
  for(int i = 0; i < bufferSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
  }
}

int ReorderBuffer::addEntry(const Type type, const int destination, const Instruction instruction) {
  buffer[head][TYPE] = type;
  buffer[head][DESTINATION] = destination;
  buffer[head][RESULT] = 0;
  buffer[head][STATUS] = ISSUED;
  instructions[head] = instruction;
  int index = head;
  head = (head + 1) % bufferSize;
  return index;
}

int ReorderBuffer::getTailIndex() const {
  return tail;
}

void ReorderBuffer::retire() {
  while(buffer[tail][STATUS] == FINISHED) {
    //buffer[tail][STATUS] = -1;
    if(buffer[tail][TYPE] == STORE_TO_REGISTER) {
      //write the result to the reorder buffer
      registerFile->setRegisterValue(buffer[tail][DESTINATION], buffer[tail][RESULT]);
      //Set the scoreBoard of the destination register to 1
      registerFile->setScoreBoardValue(buffer[tail][DESTINATION], 1);
    }
    if(buffer[tail][TYPE] == STORE_TO_MEMORY) {

    }
    if(buffer[tail][TYPE] == JUMP && buffer[tail][RESULT]) {
    *pc = buffer[tail][DESTINATION];
      *flushFlag = 1;
    }
    if(buffer[tail][TYPE] == SYSCALL) {
      *runningFlag = 0;
    }
    //print the instruction that we are retiring
    printf("Retiring Instruction: ");
    printInstruction(instructions[tail]);
    //reset the reorder buffer entry
    resetEntry(tail);
    //increment the number of instructions that we have executed
    (*noOfInstructionsExecuted)++;
    //increment the tail position
    tail = (tail + 1) % bufferSize;
  }
}

void ReorderBuffer::resetEntry(const int index) {
  instructions[index] = (Instruction) {0,0,0,0};
  for(int i = 0; i < bufferEntryFields; i++) {
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

void ReorderBuffer::setEntryResult(const int i, const int r) {
  buffer[i][RESULT] = r;
}

void ReorderBuffer::writeResult(const int i, const int r) {
  buffer[i][RESULT] = r;
}


void ReorderBuffer::flush() {
  for(int i = 0; i < bufferSize; i++) {
    buffer[i][TYPE] = -1;
    buffer[i][DESTINATION] = -1;
    buffer[i][RESULT] = -1;
    buffer[i][STATUS] = -1;
  }
  for(int i = 0; i < bufferSize; i++) {
    instructions[i] = (Instruction) {0,0,0,0};
  }
  head = 0;
  tail = 0;
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

void ReorderBuffer::getReorderBufferInstructions(Instruction* const copy) const {
  for(int i = 0; i < bufferSize; i++) {
    copy[i] = instructions[i];
  }
}

void ReorderBuffer::getReorderBufferFields(int** const copy) const {
  for(int i = 0; i < bufferSize; i++) {
    for(int j = 0; j < bufferEntryFields; j++) {
      copy[i][j] = buffer[i][j];
    }
  }
}