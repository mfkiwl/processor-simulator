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
  LoadStoreUnitReservationStation* const loadStoreUnitReservationStation, const int issueWindowSize, 
  const bool branchPrediction) :
  registerFile(registerFile),
  reorderBuffer(reorderBuffer),
  aluReservationStation(aluReservationStation),
  branchUnitReservationStation(branchUnitReservationStation),
  loadStoreUnitReservationStation(loadStoreUnitReservationStation),
  issueWindowSize(issueWindowSize),
  nextInstructions(new Instruction[issueWindowSize]),
  nextBranchAddresses(new int[issueWindowSize]),
  instructions(new Instruction[issueWindowSize]),
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
        int architecturalRegister = instructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Add instruction to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, instructions[instructionToIssue]);

        //rename the source operands
        instructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[1]);

         //update the rename table
        registerFile->setArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0], newPhysicalRegister);

        //rename destination operand
        instructions[instructionToIssue].operands[0] = newPhysicalRegister;

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
        int architecturalRegister = instructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Add instruction to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, instructions[instructionToIssue]);

        //rename the source operands
        instructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[1]);
        instructions[instructionToIssue].operands[2] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[2]);

        //update the rename table
        registerFile->setArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0], newPhysicalRegister);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;

        //rename the destination operand
        instructions[instructionToIssue].operands[0] = newPhysicalRegister;
      }
      break;

    //Load Store unit instructions
    case LW:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //get the destination architectural register
        int architecturalRegister = instructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, instructions[instructionToIssue]);
        
        //update the rename table
        registerFile->setArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0], newPhysicalRegister);

        //rename the destination operand
        instructions[instructionToIssue].operands[0] = newPhysicalRegister;

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case LWR:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //get the destination architectural register
        int architecturalRegister = instructions[instructionToIssue].operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_REGISTER, false, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, instructions[instructionToIssue]);

        //rename the source operands
        instructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[1]);

        //update the rename table
        registerFile->setArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0], newPhysicalRegister);

        //rename destination operand
        instructions[instructionToIssue].operands[0] = newPhysicalRegister;

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case SW:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_MEMORY, true, 0, 0, 0, 0, instructions[instructionToIssue]);

        //rename the registers
        instructions[instructionToIssue].operands[0] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
      break;

    case SWR:
      if(loadStoreUnitReservationStation->freeSpace()) {

        //reserve a space in the reservation station
        loadStoreUnitReservationStation->reserveSpace();

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndexes[instructionToIssue] = reorderBuffer->addEntry(STORE_TO_MEMORY, true, 0, 0, 0, 0, instructions[instructionToIssue]);

        //rename the registers
        instructions[instructionToIssue].operands[0] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);
        instructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[1]);

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

        //rename the registers
        instructions[instructionToIssue].operands[0] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[0]);
        instructions[instructionToIssue].operands[1] = registerFile->getArchitecturalRegisterMapping(instructions[instructionToIssue].operands[1]);

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

        //take note that the instruction was issued
        instructionsIssued[instructionToIssue] = true;
      }
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

void DecodeIssueUnit::moveInstructions() {
  Instruction instructionsCopy[issueWindowSize];
  int branchAddressesCopy[issueWindowSize];
  bool instructionsIssuedCopy[issueWindowSize];
  int reorderBufferIndexesCopy[issueWindowSize];
  //copy the arrays
  for(int i = 0; i < issueWindowSize; i++) {
    instructionsCopy[i] = instructions[i];
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
      branchAddresses[i] = branchAddressesCopy[index];
      instructionsIssued[i] = instructionsIssuedCopy[index];
      reorderBufferIndexes[i] = reorderBufferIndexesCopy[index];
    }
    else {
      instructions[i] = (Instruction) {0,0,0,0};
      branchAddresses[i] = -1;
      instructionsIssued[i] = true;
      reorderBufferIndexes[i] = -1;
    }
  }
}

void DecodeIssueUnit::pipe() {
  //pipe all instructions that have been issued
  for(int i = 0; i < issueWindowSize; i++) {
    if(instructionsIssued[i]) {
      pipeInstruction(i);
      instructions[i] = (Instruction) {0,0,0,0};
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
          branchAddresses[j] = nextBranchAddresses[i];
          instructionsIssued[j] = false;
          reorderBufferIndexes[j] = -1;
          break;
        }
      }
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
      //Set the scoreboard value of the destination register to zero
      registerFile->setScoreBoardValue(instructions[instructionToIssue].operands[0],0);
      aluReservationStation->setNextInstruction(instructions[instructionToIssue], 
        reorderBufferIndexes[instructionToIssue]);
      break;

    //Load Store unit instructions
    case LW:
    case LWR:
      //Set the scoreboard value of the destination register to zero
      registerFile->setScoreBoardValue(instructions[instructionToIssue].operands[0],0);
    case SW:
    case SWR:
      loadStoreUnitReservationStation->setNextInstruction(instructions[instructionToIssue], 
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
      branchUnitReservationStation->setNextInstruction(instructions[instructionToIssue], 
        reorderBufferIndexes[instructionToIssue]);
      break;
  }
}

void DecodeIssueUnit::flush() {
  //reset all necessary data 
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = (Instruction) {0,0,0,0};
    nextBranchAddresses[i] = -1;
    instructions[i] = (Instruction) {0,0,0,0};
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

//======================================================================
//getters and setters

void DecodeIssueUnit::setNextInstructions(const Instruction* const x) {
  for(int i = 0; i < issueWindowSize; i++) {
    nextInstructions[i] = x[i];
  }
}

void DecodeIssueUnit::getInstructions(Instruction* const copy) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = instructions[i];
  }
}

void DecodeIssueUnit::getReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < issueWindowSize; i++) {
    copy[i] = reorderBufferIndexes[i];
  }
}

void DecodeIssueUnit::setNextBranchAddresses(const int* const x) {
  for(int i = 0; i < issueWindowSize; i++) {
    nextBranchAddresses[i] = x[i];
  }
}