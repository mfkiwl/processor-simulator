//===========================================
//included header file containing interface
#include "load_buffer.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

#include <stdio.h>

//===========================================
//class implementation

LoadBuffer::LoadBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps) : 
  memory(memory),
  reorderBuffer(reorderBuffer),
  size(size),
  buffer(new int*[size]),
  head(0),
  tail(0),
  steps(steps)
{
  //dynamically allocated a 2d array to the read and write buffer
  for(int i = 0; i < size; i++) {
    buffer[i] = new int[entryFields];
  }
  //initialise all elements of the read and write buffer to zero
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < entryFields; j++) {
      buffer[i][j] = 0;
    }
  }
}

void LoadBuffer::addToBuffer(const int destinationRegister, const int address, const int reorderBufferIndex) {
  buffer[head][ADDRESS] = address;
  buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
  buffer[head][STEP] = 0;
  head = (head + 1) % size;
}

void LoadBuffer::incrementSteps() {
  //increment the current step for all inflight instructions in the read buffer
  for(int i = tail; i < head; i++) {
    buffer[i][STEP] += 1;
  }
}

bool LoadBuffer::readInProgress() const {
  if(head != tail) {
    return true;
  }
  else {
    return false;
  }
}

void LoadBuffer::readIfReady() {
  if(buffer[tail][STEP] > steps) {
    //get the values from the buffer entry
    int address = buffer[tail][ADDRESS];
    int reorderBufferIndex = buffer[tail][REORDER_BUFFER_INDEX];
    //get the value at the memeory address
    int value = memory->loadFromMemory(address);
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(reorderBufferIndex, value);
    //reset buffer entry
    buffer[tail][ADDRESS] = -1;
    buffer[tail][REORDER_BUFFER_INDEX] = -1;
    buffer[tail][STEP] = 0;
    //increment the tail index
    tail += 1;
  }
}

void LoadBuffer::flush() {
  for(int i = 0; i < size; i++) {
    buffer[i][ADDRESS] = -1;
    buffer[i][REORDER_BUFFER_INDEX] = -1;
    buffer[i][STEP] = 0;
  }
  head = 0;
  tail = 0;
}