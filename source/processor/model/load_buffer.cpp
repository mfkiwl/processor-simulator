//===========================================
//included header file containing interface
#include "load_buffer.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"

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

void LoadBuffer::flush() {
  for(int i = 0; i < size; i++) {
    buffer[i][DESTINATION] = 0;
    buffer[i][ADDRESS] = 0;
    buffer[i][REORDER_BUFFER_INDEX] = 0;
    buffer[i][STEP] = 0;
  }
  head = 0;
  tail = 0;
}

void LoadBuffer::stepInstructions() {
  //increment the current step for all inflight instructions in the read buffer
  for(int i = head; i < tail; i++) {
    buffer[i][STEP] += 1;
  }
}

bool LoadBuffer::waitingForReadOperation() const {
  if(head != tail) {
    return true;
  }
  else {
    return false;
  }
}

void LoadBuffer::addToBuffer(const int destinationRegister, const int address, const int reorderBufferIndex) {
  //if the start of the buffer is empty then add here
  if(head > 0) {
    head -= 1;
    buffer[head][DESTINATION] = destinationRegister;
    buffer[head][ADDRESS] = address;
    buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
    buffer[head][STEP] = 0;
  }
  //if the end of the buffer is empty then add here
  else if(tail < size - 1) {
    buffer[tail][DESTINATION] = destinationRegister;
    buffer[tail][ADDRESS] = address;
    buffer[tail][REORDER_BUFFER_INDEX] = reorderBufferIndex;
    buffer[tail][STEP] = 0;
    tail += 1;
  }
}

void LoadBuffer::readIfReady() {
  for(int i = head; i < tail; i++) {
    //for each entry check if it is ready to write
    if(buffer[i][STEP] > steps) {
      int address = buffer[i][ADDRESS];
      int value = memory->loadFromMemory(address);
      //tell the reorder buffer that we are finished executing the instruction
      reorderBuffer->finishedEntry(buffer[i][REORDER_BUFFER_INDEX], value);
      //reset write buffer entry
      buffer[i][DESTINATION] = 0;
      buffer[i][ADDRESS] = 0;
      buffer[i][STEP] = 0;
      //update the start and the end of the buffer
      if(i == head) {
        head += 1;
      }
      else if(i == tail - 1) {
        tail -= 1;
      }
    }
  }
}