//===========================================
//included header file containing interface
#include "store_buffer.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

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
    for(int j = 0; j < entryFields; j++) {
      buffer[i][j] = 0;
    }
  }
}

void StoreBuffer::flush() {
  for(int i = 0; i < size; i++) {
    buffer[i][ADDRESS] = 0;
    buffer[i][VALUE] = 0;
    buffer[i][REORDER_BUFFER_INDEX] = 0;
    buffer[i][STEP] = 0;
  }
  head = 0;
  tail = 0;
}

void StoreBuffer::addToBuffer(const int address, const int value, const int reorderBufferIndex) {
  //if the start of the buffer is empty then add here
  if(head > 0) {
    head -= 1;
    buffer[head][ADDRESS] = address;
    buffer[head][VALUE] = value;
    buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
    buffer[head][STEP] = 0;
  }
  //otherwise if the end of the buffer is empty then add here
  else if(tail < size - 1) {
    buffer[tail][ADDRESS] = address;
    buffer[tail][VALUE] = value;
    buffer[tail][REORDER_BUFFER_INDEX] = reorderBufferIndex;
    buffer[tail][STEP] = 0;
    tail += 1;
  }
}

void StoreBuffer::stepInstructions() {
  //increment the current step for all inflight instructions in the write buffer
  for(int i = head; i < tail; i++) {
    buffer[i][STEP] += 1;
  }
}

bool StoreBuffer::waitingForStore() const {
  if(tail != head) {
    return true;
  }
  else {
    return false;
  }
}

void StoreBuffer::writeIfReady() {
  for(int i = head; i < tail; i++) {
    //for each entry check if it is ready to write
    if(buffer[i][STEP] >= steps) {
      //write the value to the memory address
      int address = buffer[i][ADDRESS];
      int value = buffer[i][VALUE];
      memory->storeInMemory(address, value);
      //printf("STORED %d at ADDRESS %d\n", value, address);
      //tell the reorder buffer that we are finished executing the instruction
      reorderBuffer->finishedEntry(buffer[i][REORDER_BUFFER_INDEX], 0);
      //reset write buffer entry
      buffer[i][ADDRESS] = 0;
      buffer[i][VALUE] = 0;
      buffer[i][REORDER_BUFFER_INDEX] = 0;
      buffer[i][STEP] = 0;
      //update the start and end of the buffer
      if(i == head) {
        head += 1;
      }    
      else if(i == tail - 1) {
        tail -= 1;
      }
    }
  }
}