//===========================================
// included header file containing interface
#include "alu.h"

//===========================================
// included dependencies
#include "register_file.h"
#include "reorder_buffer.h"
#include "constants.h"

//===========================================
// class implementation

ALU::ALU(RegisterFile* registerFile, ReorderBuffer* reorderBuffer) : 
  registerFile(registerFile),
  reorderBuffer(reorderBuffer),
  opcode(0),
  destinationRegister(-1),
  result(0),
  nextReorderBufferIndex(-1),
  currentReorderBufferIndex(-1)
{
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
}

void ALU::execute() {
  if(nextReorderBufferIndex != -1) {
    //tell reorder buffer that we are executing the instruction
    reorderBuffer->executingEntry(nextReorderBufferIndex);
    //execute the instruction
    destinationRegister = operands[0];
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

    currentReorderBufferIndex = nextReorderBufferIndex;

    //reset inputs
    opcode = 0;
    for(int i = 0; i < 3; i++) {
      operands[i] = 0;
    }
  }
}

void ALU::pipe() {
  if(destinationRegister != -1) {
    //tell the reorder buffer that we are finished executing the instruction
    reorderBuffer->finishedEntry(currentReorderBufferIndex, result);
    //reset variables
    destinationRegister = -1;
  }
}

void ALU::setOpcode(int x) {
  opcode = x;
}

void ALU::setOperands(int x[3]) {
  for(int i = 0; i < 3; i++) {
    operands[i] = x[i];
  }
}

void ALU::flush() {
  opcode = 0;
  for(int i = 0; i < 3; i++) {
    operands[i] = 0;
  }
  nextReorderBufferIndex = -1;
  currentReorderBufferIndex = -1;
  destinationRegister = -1;
  result = 0;
}

void ALU::setReorderBufferIndex(int i) {
  nextReorderBufferIndex = i;
}