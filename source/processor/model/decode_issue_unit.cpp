//===========================================
//included header file containing interface
#include "decode_issue_unit.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "register_file.h"
#include "reorder_buffer.h"
#include "alu_reservation_station.h"
#include "branch_unit_reservation_station.h"
#include "store_queue.h"
#include "load_queue.h"
#include "instructions.h"
#include "constants.h"

//===========================================
//class implementation

//=================================================================================================================
//public functions

DecodeIssueUnit::DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, 
  ALUReservationStation* const aluReservationStation, BranchUnitReservationStation* const branchUnitReservationStation, 
  StoreQueue* const storeQueue, LoadQueue* const loadQueue, const int issueWindowSize, 
  const bool branchPrediction) :
  registerFile(registerFile),
  reorderBuffer(reorderBuffer),
  aluReservationStation(aluReservationStation),
  branchUnitReservationStation(branchUnitReservationStation),
  storeQueue(storeQueue),
  loadQueue(loadQueue),
  issueWindowSize(issueWindowSize),
  nextInstructions(new Instruction[issueWindowSize]),
  nextBranchAddresses(new int[issueWindowSize]),
  instructions(new Instruction[issueWindowSize]),
  operandTypes(new OperandType*[issueWindowSize]),
  branchAddresses(new int[issueWindowSize]),
  instructionsIssued(new bool[issueWindowSize]),
  reorderBufferIndexes(new int[issueWindowSize]),
  branchPrediction(branchPrediction)
{
  //initialise arrays
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextBranchAddresses[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    operandTypes[i] = new OperandType[numOfOperands];
    for(int j = 0; j < numOfOperands; j++) {
      operandTypes[i][j] = NONE;
    }
    instructionsIssued[i] = true;
    reorderBufferIndexes[i] = -1;
    branchAddresses[i] = -1;
  }
}

void DecodeIssueUnit::execute() {
  //try to issue all instructions in order
  for(int i = 0; i < issueWindowSize; i++) {
    //if there is space in the reorder buffer
    if(reorderBuffer->freeSpace()) {
      //try and issue instruction
      issue(i);
      //if you can't issue an instruction then stop issuing
      if(instructionsIssued[i] != true) {
        break;
      }
    }
  }
}

void DecodeIssueUnit::issue(int instructionToIssue) {
  switch(instructions[instructionToIssue].opcode) {

    //NOOP instruction
    case NOOP:
      //take note that the instruction was issued
      instructionsIssued[instructionToIssue] = true;
      break;

    //ALU instructions
    case ADDI:
      //if there is space in the reservation then issue the instruction
      if(aluReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        aluReservationStation->reserveSpace();

        //get the destination architectural register
        int destinationRegister = instructions[instructionToIssue].operands[0];

        //Add instruction to the reorder buffer
        int reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, destinationRegister, 0, 0, 
          instructions[instructionToIssue]);

        //Add the reorder buffer index to the list of current instructions
        reorderBufferIndexes[instructionToIssue] = reorderBufferIndex;

        //set the first operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[1])) {
          operandTypes[instructionToIssue][1] = ROB;
        }
        else {
          operandTypes[instructionToIssue][1] = REGISTER;
        }

        //rename the first operand
        instructions[instructionToIssue].operands[1] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[1]);

        //set the second operands type
        operandTypes[instructionToIssue][2] = CONSTANT;

        //set the destinations type
        operandTypes[instructionToIssue][0] = ROB;

        //rename the destination
        instructions[instructionToIssue].operands[0] = reorderBufferIndex;

        //update the rename table for the destination register
        registerFile->setMappingToRobEntry(destinationRegister, reorderBufferIndex);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:

      //if there is space in the reservation then issue the instruction
      if(aluReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        aluReservationStation->reserveSpace();

        //get the destination architectural register
        int destinationRegister = instructions[instructionToIssue].operands[0];

        //Add instruction to the reorder buffer
        int reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, destinationRegister, 0, 0, 
          instructions[instructionToIssue]);

        //Add the reorder buffer index to the list of current instructions
        reorderBufferIndexes[instructionToIssue] = reorderBufferIndex;

        //set the first operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[1])) {
          operandTypes[instructionToIssue][1] = ROB;
        }
        else {
          operandTypes[instructionToIssue][1] = REGISTER;
        }

        //rename the first operand
        instructions[instructionToIssue].operands[1] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[1]);

        //set the  second operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[2])) {
          operandTypes[instructionToIssue][2] = ROB;
        }
        else {
          operandTypes[instructionToIssue][2] = REGISTER;
        }

        //rename the second operand
        instructions[instructionToIssue].operands[2] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[2]);

        //set the desination type
        operandTypes[instructionToIssue][0] = ROB;

        //rename the destination
        instructions[instructionToIssue].operands[0] = reorderBufferIndex;

        //update the rename table for the destination register
        registerFile->setMappingToRobEntry(destinationRegister, reorderBufferIndex);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    //Load Store unit instructions
    case LW:
      if(loadQueue->freeSpace()) {

        //reserve a space in the reservation station
        loadQueue->reserveSpace();

        //get the destination architectural register
        int destinationRegister = instructions[instructionToIssue].operands[0];

        //Instruction has been issued so add entry to the reorder buffer
        int reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, 
          destinationRegister, 0, 0, instructions[instructionToIssue]);
        reorderBufferIndexes[instructionToIssue] = reorderBufferIndex;

        //set the first operand type
        operandTypes[instructionToIssue][1] = CONSTANT;

        //set the destination type
        operandTypes[instructionToIssue][0] = ROB;

        //rename the destination register
        instructions[instructionToIssue].operands[0] = reorderBufferIndex;

        //update the rename table
        registerFile->setMappingToRobEntry(destinationRegister, reorderBufferIndex);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case LWR:
      if(loadQueue->freeSpace()) {

        //reserve a space in the reservation station
        loadQueue->reserveSpace();

        //get the destination architectural register
        int destinationRegister = instructions[instructionToIssue].operands[0];

        //Instruction has been issued so add entry to the reorder buffer
        int reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, destinationRegister, 0, 0, 
          instructions[instructionToIssue]);

        //store the reorder buffer index for the instruction
        reorderBufferIndexes[instructionToIssue] = reorderBufferIndex;

        //set the first operand
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[1])) {
          operandTypes[instructionToIssue][1] = ROB;
        }
        else {
          operandTypes[instructionToIssue][1] = REGISTER;
        }

        //rename the first operand
        instructions[instructionToIssue].operands[1] = registerFile->getRegisterMapping(instructions[instructionToIssue].operands[1]);

        //set the destination type
        operandTypes[instructionToIssue][0] = ROB;

        //rename the destination register
        instructions[instructionToIssue].operands[0] = reorderBufferIndex;

        //update the rename table
        registerFile->setMappingToRobEntry(destinationRegister, reorderBufferIndex);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case SW:
      if(storeQueue->freeSpace()) {

        //reserve a space in the reservation station
        storeQueue->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_MEMORY, true, 0, 0, 0, 0, instructions[instructionToIssue]);

        //set the operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[0])) {
          operandTypes[instructionToIssue][0] = ROB;
        }
        else {
          operandTypes[instructionToIssue][0] = REGISTER;
        }

        //rename the operand
        instructions[instructionToIssue].operands[0] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[0]);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case SWR:
      if(storeQueue->freeSpace()) {

        //reserve a space in the reservation station
        storeQueue->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_MEMORY, true, 0, 0, 0, 0, instructions[instructionToIssue]);

        //set the first operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[0])) {
          operandTypes[instructionToIssue][0] = ROB;
        }
        else {
          operandTypes[instructionToIssue][0] = CONSTANT;
        }

        //rename first operand
        instructions[instructionToIssue].operands[0] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[0]);

        //set the second operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[1])) {
          operandTypes[instructionToIssue][1] = ROB;
        }
        else {
          operandTypes[instructionToIssue][1] = REGISTER;
        }

        //rename the second operand
        instructions[instructionToIssue].operands[1] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[1]);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    //Branch unit instructions
    case BEQ:
    case BNE:
      if(branchUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        branchUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        if(branchPrediction) {
          reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(JUMP, true, 
            branchAddresses[instructionToIssue], 0, 0, 0, instructions[instructionToIssue]);
        }
        else {
          reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(JUMP, false,
            branchAddresses[instructionToIssue], 0, 0, 0, instructions[instructionToIssue]);
        }

        //set the first operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[0])) {
          operandTypes[instructionToIssue][0] = ROB;
        }
        else {
          operandTypes[instructionToIssue][0] = REGISTER;
        }

        //rename the first operand
        instructions[instructionToIssue].operands[0] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[0]);

        //set the second operand type
        if(registerFile->isRobMapping(instructions[instructionToIssue].operands[1])) {
          operandTypes[instructionToIssue][1] = ROB;
        }
        else {
          operandTypes[instructionToIssue][1] = REGISTER;
        }

        //rename the second operand
        instructions[instructionToIssue].operands[1] = 
          registerFile->getRegisterMapping(instructions[instructionToIssue].operands[1]);

        //set the third operand type
        operandTypes[instructionToIssue][2] = CONSTANT;

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      break;
    case J:
    case JR:
      break;
                    
    //Instruction to finish the program
    case HALT:
      if(branchUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        branchUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(SYSCALL, false, 0, 0, 0, 0, instructions[instructionToIssue]);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;
  }
}

void DecodeIssueUnit::pipe() {
  //pipe all instructions that have been issued
  for(int i = 0; i < issueWindowSize; i++) {
    if(instructionsIssued[i]) {
      pipeInstruction(i);
      instructions[i] = (Instruction) {0,0,0,0};
      for(int j = 0; j < numOfOperands; j++) {
        operandTypes[i][j] = NONE;
      }
      branchAddresses[i] = -1;
      instructionsIssued[i] = false;
      reorderBufferIndexes[i] = -1;
    }
  }
  //move the instructions so that they start at the top
  moveInstructions();
  //set the current instructions equal to the next instructions
  for(int i = 0; i < issueWindowSize; i++) {
    if(nextInstructions[i].opcode != NOOP) {
      for(int j = 0; j < issueWindowSize; j++) {
        if(instructions[j].opcode == NOOP) {
          instructions[j] = nextInstructions[i];
          setOperandTypes(j);
          branchAddresses[j] = nextBranchAddresses[i];
          instructionsIssued[j] = false;
          reorderBufferIndexes[j] = -1;
          break;
        }
      }
    }
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextBranchAddresses[i] = -1;
  }
}

void DecodeIssueUnit::flush() {
  //reset all necessary data 
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextBranchAddresses[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
    for(int j = 0; j < numOfOperands; j++) {
      operandTypes[i][j] = NONE;
    }
    instructionsIssued[i] = true;
    reorderBufferIndexes[i] = -1;
    branchAddresses[i] = -1;
  }
}

void DecodeIssueUnit::print() const {
  printf("DECODE ISSUE UNIT:");
  for(int i = 0; i < issueWindowSize; i++) {
    printInstruction(instructions[i]);
  }
}

bool DecodeIssueUnit::allInstructionsIssued() const {
  for(int i = 0; i < issueWindowSize; i++) {
    if(!instructionsIssued[i]) {
      return false;
    }
  }
  return true;
}

int DecodeIssueUnit::numFreeSpaces() const {
  int count = 0;
  for(int i = 0; i < issueWindowSize; i++) {
    if(instructionsIssued[i]) {
      count++;
    }
  }
  return count;
}

//===============================================================================================================
//private functions

void DecodeIssueUnit::setOperandTypes(const int index) {
  switch(instructions[index].opcode) {
    case NOOP:
      operandTypes[index][0] = NONE;
      operandTypes[index][1] = NONE;
      operandTypes[index][2] = NONE;
      break;
    case ADDI:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = REGISTER;
      operandTypes[index][2] = CONSTANT;
      break;
    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = REGISTER;
      operandTypes[index][2] = REGISTER;
      break;
    case LW:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = CONSTANT;
      operandTypes[index][2] = NONE;
      break;
    case LWR:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = REGISTER;
      operandTypes[index][2] = NONE;
      break;
    case SW:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = CONSTANT;
      operandTypes[index][2] = NONE;
      break;
    case SWR:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = REGISTER;
      operandTypes[index][2] = NONE;
      break;
    case BEQ:
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      break;
    case BNE:
      operandTypes[index][0] = REGISTER;
      operandTypes[index][1] = REGISTER;
      operandTypes[index][2] = CONSTANT;
      break;
    case J:
    case JR:
    case HALT:
      operandTypes[index][0] = NONE;
      operandTypes[index][1] = NONE;
      operandTypes[index][2] = NONE;
      break;
  }
}

void DecodeIssueUnit::moveInstructions() {
  Instruction instructionsCopy[issueWindowSize];
  OperandType operandTypesCopy[issueWindowSize][numOfOperands];
  int branchAddressesCopy[issueWindowSize];
  bool instructionsIssuedCopy[issueWindowSize];
  int reorderBufferIndexesCopy[issueWindowSize];
  //copy the arrays
  for(int i = 0; i < issueWindowSize; i++) {
    instructionsCopy[i] = instructions[i];
    for(int j = 0; j < numOfOperands; j++) {
      operandTypesCopy[i][j] = operandTypes[i][j];
    }
    branchAddressesCopy[i] = branchAddresses[i];
    instructionsIssuedCopy[i] = instructionsIssued[i];
    reorderBufferIndexesCopy[i] = reorderBufferIndexes[i];
  }
  //find the index of the first instruction
  int start = 0;
  for(int i = 0; i < issueWindowSize; i++) {
    if(instructions[i].opcode != NOOP) {
      start = i;
      break;
    }
  }
  //move the instructions up
  for(int i = 0; i < issueWindowSize; i++) {
    int index = start + i;
    if(index < issueWindowSize) {
      instructions[i] = instructionsCopy[index];
      for(int j = 0; j < numOfOperands; j++) {
        operandTypes[i][j] = operandTypesCopy[index][j];
      }
      branchAddresses[i] = branchAddressesCopy[index];
      instructionsIssued[i] = instructionsIssuedCopy[index];
      reorderBufferIndexes[i] = reorderBufferIndexesCopy[index];
    }
    else {
      instructions[i] = (Instruction) {0,0,0,0};
      for(int j = 0; j < numOfOperands; j++) {
        operandTypes[i][j] = NONE;
      }
      branchAddresses[i] = -1;
      instructionsIssued[i] = true;
      reorderBufferIndexes[i] = -1;
    }
  }
}

void DecodeIssueUnit::pipeInstruction(int instructionToIssue) {
  //send the current instruction to the necessary component
  switch(instructions[instructionToIssue].opcode) {

    //NOOP instruction
    case NOOP:
      break;

    //ALU instructions
    case ADD:
    case ADDI:
    case AND:
    case MULT:
    case OR:
    case SUB:
      //send the issued instruction to the alu reservation station
      aluReservationStation->setNextInstruction(instructions[instructionToIssue], operandTypes[instructionToIssue], 
        reorderBufferIndexes[instructionToIssue]);
      break;

    //Load Store unit instructions
    case LW:
    case LWR:
      //send the issued instruction to the load store unit reservation station
      loadQueue->setNextInstruction(instructions[instructionToIssue], operandTypes[instructionToIssue],
        reorderBufferIndexes[instructionToIssue]);
      break;
    case SW:
    case SWR:
      //send the issued instruction to the load store unit reservation station
      storeQueue->setNextInstruction(instructions[instructionToIssue], operandTypes[instructionToIssue],
        reorderBufferIndexes[instructionToIssue]);
      break;

    //Branch unit instructions
    case BEQ:
    case BNE:
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
    case J:
    case JR:

    //Instruction to finish the program
    case HALT:
      //send the issued instruction to the branch unit reservation station
      branchUnitReservationStation->setNextInstruction(instructions[instructionToIssue], 
        operandTypes[instructionToIssue], reorderBufferIndexes[instructionToIssue]);
      break;
  }
}

//======================================================================
//getters and setters

void DecodeIssueUnit::setNextInstructions(const Instruction x[]) {
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = x[i];
  }
}

void DecodeIssueUnit::getInstructions(Instruction copy[]) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = instructions[i];
  }
}

void DecodeIssueUnit::getReorderBufferIndexes(int copy[]) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void DecodeIssueUnit::setNextBranchAddresses(const int x[]) {
  for(int i = 0; i < issueWindowSize; i++) {
    nextBranchAddresses[i] = x[i];
  }
}

void DecodeIssueUnit::getOperandTypes(OperandType copy[][numOfOperands]) const {
  for(int i = 0; i < issueWindowSize; i++) {
    for(int j = 0; j < numOfOperands; j++) {
      copy[i][j] = operandTypes[i][j];
    }
  }
}