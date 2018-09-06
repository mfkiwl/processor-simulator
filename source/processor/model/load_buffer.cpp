//===========================================
//included header file containing interface
#include "load_buffer.h"

//===========================================
// included dependencies
#include "memory.h"
#include "reorder_buffer.h"
#include "alu_reservation_station.h"

//===========================================
//class implementation

LoadBuffer::LoadBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps,
  ALUReservationStation* const aluReservationStation) : 
  memory(memory),
  reorderBuffer(reorderBuffer),
  size(size),
  buffer(new int*[size]),
  head(0),
  tail(0),
  steps(steps),
  aluReservationStation(aluReservationStation)
{
  //dynamically allocated a 2d array to the read and write buffer
  for(int i = 0; i < size; i++) {
    buffer[i] = new int[entryFields];
  }
  //initialise all elements of the read and write buffer to zero
  for(int i = 0; i < size; i++) {
    buffer[i][ADDRESS] = -1;
    buffer[i][REORDER_BUFFER_INDEX] = -1;
    buffer[i][STEP] = -1;
  }
}

void LoadBuffer::addToBuffer(const int destinationRegister, const int address, const int reorderBufferIndex) {
  buffer[head][ADDRESS] = address;
  buffer[head][DESTINATION_REGISTER] = destinationRegister;
  buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
  buffer[head][STEP] = 1;
  head = (head + 1) % size;
}

void LoadBuffer::incrementSteps() {
  //increment the current step for all inflight instructions in the read buffer
  int i = tail;
  while(i != head) {
    buffer[i][STEP]++;
    i = (i + 1) % size;
  }
}

void LoadBuffer::readIfReady() {
  if(buffer[tail][STEP] >= steps) {
    //get the values from the buffer entry
    int address = buffer[tail][ADDRESS];
    int reorderBufferIndex = buffer[tail][REORDER_BUFFER_INDEX];
    //get the value at the memeory address
    int value = memory->loadFromMemory(address);
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(reorderBufferIndex, value);
    //aluReservationStation->broadcast(buffer[tail][DESTINATION_REGISTER], value);
    //reset buffer entry
    buffer[tail][ADDRESS] = -1;
    buffer[tail][DESTINATION_REGISTER] = -1;
    buffer[tail][REORDER_BUFFER_INDEX] = -1;
    buffer[tail][STEP] = -1;
    //increment the tail index
    tail = (tail + 1) % size;
  }
}

void LoadBuffer::flush() {
  for(int i = 0; i < size; i++) {
    buffer[i][ADDRESS] = -1;
    buffer[i][DESTINATION_REGISTER] = -1;
    buffer[i][REORDER_BUFFER_INDEX] = -1;
    buffer[i][STEP] = -1;
  }
  head = 0;
  tail = 0;
}