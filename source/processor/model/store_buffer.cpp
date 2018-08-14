//===========================================
//included header file containing interface
#include "store_buffer.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

#include <stdio.h>

//===========================================
//class implementation

StoreBuffer::StoreBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps) : 
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
    buffer[i][ADDRESS] = -1;
    buffer[i][VALUE] = -1;
    buffer[i][REORDER_BUFFER_INDEX] = -1;
    buffer[i][STEP] = -1;
  }
}

void StoreBuffer::addToBuffer(const int address, const int value, const int reorderBufferIndex) {
  buffer[head][ADDRESS] = address;
  buffer[head][VALUE] = value;
  buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
  buffer[head][STEP] = 1;
  head = (head + 1) % size;
}

void StoreBuffer::incrementSteps() {
  //increment the current step for all inflight instructions in the write buffer
  int i = tail;
  while(i != head) {
    buffer[i][STEP]++;
    i = (i + 1) % size;
  }
}

void StoreBuffer::writeIfReady() {
  if(buffer[tail][STEP] >= steps) {
    //get the value from the buffer entry
    int address = buffer[tail][ADDRESS];
    int value = buffer[tail][VALUE];
    int reorderBufferIndex = buffer[tail][REORDER_BUFFER_INDEX];
    //store the value at the memory address
    memory->storeInMemory(address, value);
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(reorderBufferIndex, value);
    //reset write buffer entry
    buffer[tail][ADDRESS] = -1;
    buffer[tail][VALUE] = -1;
    buffer[tail][REORDER_BUFFER_INDEX] = -1;
    buffer[tail][STEP] = -1;
    //update the start and end of the buffer
    tail = (tail + 1) % size;
  }
}

void StoreBuffer::flush() {
  for(int i = 0; i < size; i++) {
    buffer[i][ADDRESS] = -1;
    buffer[i][VALUE] = -1;
    buffer[i][REORDER_BUFFER_INDEX] = -1;
    buffer[i][STEP] = -1;
  }
  head = 0;
  tail = 0;
}

void StoreBuffer::print() const {
  printf("tail: %d\n", tail);
  printf("head: %d\n", head);
  for(int i = 0; i < size; i++) {
    printf("%d ", buffer[i][REORDER_BUFFER_INDEX]);
  }
  printf("\n");
}