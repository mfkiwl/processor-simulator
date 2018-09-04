//===========================================
// included header file containing interface
#include "alu.h"

//===========================================
// included dependencies
#include "register_file.h"
#include "reorder_buffer.h"
#include "alu_reservation_station.h"
#include "store_queue.h"
#include "load_queue.h"
#include "branch_unit_reservation_station.h"
#include "constants.h"

#include <stdio.h>

//===========================================
// class implementation

//===============================================================
//public functions

ALU::ALU() :
  reorderBuffer(NULL),
  aluReservationStation(NULL),
  storeQueue(NULL),
  loadQueue(NULL),
  branchUnitReservationStation(NULL),
  nextOpcode(0),
  nextReorderBufferIndex(-1),
  opcode(0),
  reorderBufferIndex(-1),
  result(0)
{
  for(int i = 0; i < numOfOperands; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
}

ALU::ALU(ReorderBuffer* const reorderBuffer, ALUReservationStation* const aluReservationStation) : 
  reorderBuffer(reorderBuffer),
  aluReservationStation(aluReservationStation),
  storeQueue(NULL),
  loadQueue(NULL),
  nextOpcode(0),
  nextReorderBufferIndex(-1),
  opcode(0),
  reorderBufferIndex(-1),
  result(0)
{
  for(int i = 0; i < numOfOperands; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
}

void ALU::execute() {
  if(opcode != NOOP) {
    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(reorderBufferIndex);
    //execute the instruction
    switch(opcode) {
      case ADD: case ADDI:
        result = operands[1] + operands[2];
        break;
      case AND:
        result = operands[1] && operands[2];
        break;
      case MULT:
        result = operands[1] * operands[2];
        break;
      case OR:
        result = operands[1] || operands[2];
        break;
     case SUB:
        result = operands[1] - operands[2];
        break;
    }
    //tell the reorder buffer the result
    reorderBuffer->finishedEntry(reorderBufferIndex, result);
    aluReservationStation->broadcast(operands[0], result);
    storeQueue->broadcast(operands[0], result);
    loadQueue->broadcast(operands[0], result);
    branchUnitReservationStation->broadcast(operands[0], result);
  }
}

void ALU::pipe() {
  
  //reset the  result to zero
  result = 0;

  //set the  values equal to the next values
  opcode = nextOpcode;
  for(int i = 0; i < numOfOperands; i++) {
    operands[i] = nextOperands[i]; 
  }
  reorderBufferIndex = nextReorderBufferIndex;

  //reset the next values
  nextOpcode = 0;
  for(int i = 0; i < numOfOperands; i++) {
    nextOperands[i] = 0;
  }
  nextReorderBufferIndex = -1;
}

//========================================================================
//getters and setters

void ALU::setNextOpcode(const int x) {
  nextOpcode = x;
}

void ALU::setNextOperands(const int x[numOfOperands]) {
  for(int i = 0; i < numOfOperands; i++) {
    nextOperands[i] = x[i];
  }
}

void ALU::setNextReorderBufferIndex(const int i) {
  nextReorderBufferIndex = i;
}

void ALU::flush() {
  nextOpcode = 0;
  opcode = 0;
  for(int i = 0; i < numOfOperands; i++) {
    nextOperands[i] = 0;
    operands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  reorderBufferIndex = -1;
  result = 0;
}

int ALU::getResult() const {
  return result;
}

int ALU::getReorderBufferIndex() const {
  return reorderBufferIndex;
}

void ALU::setReorderBufferPointer(ReorderBuffer* p) {
  reorderBuffer = p;
}

void ALU::setALUReservationStationPointer(ALUReservationStation* p) {
  aluReservationStation = p;
}

void ALU::setStoreQueuePointer(StoreQueue* p) {
  storeQueue = p;
}

void ALU::setLoadQueuePointer(LoadQueue* p) {
  loadQueue = p;
}

void ALU::setBranchUnitReservationStationPointer(BranchUnitReservationStation* p) {
  branchUnitReservationStation = p;
}