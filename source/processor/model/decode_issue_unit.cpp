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
#include "load_store_unit_reservation_station.h"
#include "instructions.h"

//===========================================
//class implementation

DecodeIssueUnit::DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, 
  ALUReservationStation* const aluReservationStation, BranchUnitReservationStation* const branchUnitReservationStation, 
  LoadStoreUnitReservationStation* const loadStoreUnitReservationStation, const int issueWindowSize) :
  registerFile(registerFile),
  reorderBuffer(reorderBuffer),
  aluReservationStation(aluReservationStation),
  branchUnitReservationStation(branchUnitReservationStation),
  loadStoreUnitReservationStation(loadStoreUnitReservationStation),
  issueWindowSize(issueWindowSize),
  nextInstructions(new Instruction[issueWindowSize]),
  currentInstructions(new Instruction[issueWindowSize]),
  currentInstructionsIssued(new bool[issueWindowSize]),
  reorderBufferIndexes(new int[issueWindowSize])
{
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    currentInstructions[i] = (Instruction) {0,0,0,0};
    currentInstructionsIssued[i] = true;
    reorderBufferIndexes[i] = -1;
  }
}

void DecodeIssueUnit::execute() {
  for(int i = 0; i < issueWindowSize; i++) {
    if(reorderBuffer->freeSpace()) {
      issue(i);
      if(currentInstructionsIssued[i] != true) {
        break;
      }
    }
  }
}

void DecodeIssueUnit::issue(int instructionToIssue) {
  switch(currentInstructions[instructionToIssue].opcode) {

    //NOOP instruction
    case NOOP:
      //take note that the instruction was issued
      currentInstructionsIssued[instructionToIssue] = true;
      break;

    //ALU instructions
    case ADDI:
      //if there is space in the reservation then issue the instruction
      if(aluReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        aluReservationStation->reserveSpace();

        //get the destination architectural register
        int architecturalRegister = currentInstructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Add instruction to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstructions[instructionToIssue]);

        //rename the source operands
        currentInstructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[1]);

         //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0], newPhysicalRegister);

        //rename destination operand
        currentInstructions[instructionToIssue].operands[0] = newPhysicalRegister;

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
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
        int architecturalRegister = currentInstructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Add instruction to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstructions[instructionToIssue]);

        //rename the source operands
        currentInstructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[1]);
        currentInstructions[instructionToIssue].operands[2] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[2]);

        //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0], newPhysicalRegister);

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;

        //rename the destination operand
        currentInstructions[instructionToIssue].operands[0] = newPhysicalRegister;
      }
      break;

    //Load Store unit instructions
    case LW:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //get the destination architectural register
        int architecturalRegister = currentInstructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstructions[instructionToIssue]);
        
        //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0], newPhysicalRegister);

        //rename the destination operand
        currentInstructions[instructionToIssue].operands[0] = newPhysicalRegister;

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;

    case LWR:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //get the destination architectural register
        int architecturalRegister = currentInstructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstructions[instructionToIssue]);

        //rename the source operands
        currentInstructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[1]);

        //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0], newPhysicalRegister);

        //rename destination operand
        currentInstructions[instructionToIssue].operands[0] = newPhysicalRegister;

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;

    case SW:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_MEMORY, 0, 0, 0, 0, currentInstructions[instructionToIssue]);

        //rename the registers
        currentInstructions[instructionToIssue].operands[0] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;

    case SWR:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_MEMORY, 0, 0, 0, 0, currentInstructions[instructionToIssue]);

        //rename the registers
        currentInstructions[instructionToIssue].operands[0] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);
        currentInstructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[1]);

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;

    //Branch unit instructions
    case BEQ:
    case BNE:
      if(branchUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        branchUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(JUMP, currentInstructions[instructionToIssue].operands[2], 0, 0, 0, currentInstructions[instructionToIssue]);

        //rename the registers
        currentInstructions[instructionToIssue].operands[0] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[0]);
        currentInstructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstructions[instructionToIssue].operands[1]);

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;
    case BGEZ:
    case BGTZ:
    case BLEZ:
    case BLTZ:
      break;
    case J:
    case JR:
      if(branchUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        branchUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(JUMP, currentInstructions[instructionToIssue].operands[0], 0, 0, 0, currentInstructions[instructionToIssue]);

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;
                    
    //Instruction to finish the program
    case HALT:
      if(branchUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        branchUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(SYSCALL, 0, 0, 0, 0, currentInstructions[instructionToIssue]);

        //take note that the instruction was issued
        currentInstructionsIssued[instructionToIssue] = true;
      }
      break;
  }
}

void DecodeIssueUnit::moveInstructions() {
  Instruction currentInstructionsCopy[issueWindowSize];
  bool currentInstructionsIssuedCopy[issueWindowSize];
  int reorderBufferIndexesCopy[issueWindowSize];
  //copy the arrays
  for(int i = 0; i < issueWindowSize; i++) {
    currentInstructionsCopy[i] = currentInstructions[i];
    currentInstructionsIssuedCopy[i] = currentInstructionsIssued[i];
    reorderBufferIndexesCopy[i] = reorderBufferIndexes[i];
  }
  //find the index of the first instruction
  int start = 0;
  for(int i = 0; i < issueWindowSize; i++) {
    if(currentInstructions[i].opcode != NOOP) {
      start = i;
      break;
    }
  }
  //move the instructions up
  for(int i = 0; i < issueWindowSize; i++) {
    int index = start + i;
    if(index < issueWindowSize) {
      currentInstructions[i] = currentInstructionsCopy[index];
      currentInstructionsIssued[i] = currentInstructionsIssuedCopy[index];
      reorderBufferIndexes[i] = reorderBufferIndexesCopy[index];
    }
    else {
      currentInstructions[i] = (Instruction) {0,0,0,0};
      currentInstructionsIssued[i] = true;
      reorderBufferIndexes[i] = -1;
    }
  }
}

void DecodeIssueUnit::pipe() {
  for(int i = 0; i < issueWindowSize; i++) {
    if(currentInstructionsIssued[i]) {
      pipeInstruction(i);
      currentInstructions[i] = (Instruction) {0,0,0,0};
      currentInstructionsIssued[i] = false;
      reorderBufferIndexes[i] = -1;
    }
  }
  //move the instructions so that they start at the top
  moveInstructions();
  //set the current instructions equal to the next instructions
  for(int i = 0; i < issueWindowSize; i++) {
    if(nextInstructions[i].opcode != NOOP) {
      for(int j = 0; j < issueWindowSize; j++) {
        if(currentInstructions[j].opcode == NOOP) {
          currentInstructions[j] = nextInstructions[i];
          currentInstructionsIssued[j] = false;
          reorderBufferIndexes[j] = -1;
          break;
        }
      }
    }
  }
}

void DecodeIssueUnit::pipeInstruction(int instructionToIssue) {
  //send the current instruction to the necessary component
  switch(currentInstructions[instructionToIssue].opcode) {

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
      //Set the scoreboard value of the destination register to zero
      registerFile->setScoreBoardValue(currentInstructions[instructionToIssue].operands[0],0);
      aluReservationStation->setNextInstruction(currentInstructions[instructionToIssue], 
        reorderBufferIndexes[instructionToIssue]);
      break;

    //Load Store unit instructions
    case LW:
    case LWR:
      //Set the scoreboard value of the destination register to zero
      registerFile->setScoreBoardValue(currentInstructions[instructionToIssue].operands[0],0);
    case SW:
    case SWR:
      loadStoreUnitReservationStation->setNextInstruction(currentInstructions[instructionToIssue], 
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
      //branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
      branchUnitReservationStation->setNextInstruction(currentInstructions[instructionToIssue], 
        reorderBufferIndexes[instructionToIssue]);
      break;
  }
}

void DecodeIssueUnit::flush() {
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    currentInstructions[i] = (Instruction) {0,0,0,0};
    currentInstructionsIssued[i] = true;
    reorderBufferIndexes[i] = -1;
  }
}

void DecodeIssueUnit::print() const {
  printf("DECODE ISSUE UNIT:");
  for(int i = 0; i < issueWindowSize; i++) {
    printInstruction(currentInstructions[i]);    
  }
}

bool DecodeIssueUnit::allInstructionsIssued() const {
  for(int i = 0; i < issueWindowSize; i++) {
    if(!currentInstructionsIssued[i]) {
      return false;
    }
  }
  return true;
}

int DecodeIssueUnit::numFreeSpaces() const {
  int count = 0;
  for(int i = 0; i < issueWindowSize; i++) {
    if(currentInstructionsIssued[i]) {
      count++;
    }
  }
  return count;
}

//======================================================================
//getters and setters

void DecodeIssueUnit::setNextInstructions(const Instruction* const x) {
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = x[i];
  }
}

void DecodeIssueUnit::getInstructions(Instruction* const copy) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = currentInstructions[i];
  }
}

void DecodeIssueUnit::getReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}