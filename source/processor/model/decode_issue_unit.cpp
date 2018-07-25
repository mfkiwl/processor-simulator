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

DecodeIssueUnit::DecodeIssueUnit(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, ALUReservationStation* const aluReservationStation, BranchUnitReservationStation* const branchUnitReservationStation, LoadStoreUnitReservationStation* const loadStoreUnitReservationStation) :
  registerFile(registerFile),
  reorderBuffer(reorderBuffer),
  aluReservationStation(aluReservationStation),
  branchUnitReservationStation(branchUnitReservationStation),
  loadStoreUnitReservationStation(loadStoreUnitReservationStation),
  nextInstruction((Instruction) {0,0,0,0}),
  currentInstruction((Instruction) {0,0,0,0}),
  reorderBufferIndex(-1),
  blockingFlag(false)
{}

void DecodeIssueUnit::execute() {
  if(reorderBuffer->freeSpace()) {
    issue();
  }
  else {
    blockingFlag = true;
  }
}

void DecodeIssueUnit::issue() {
  switch(currentInstruction.opcode) {

    //NOOP instruction
    case NOOP:
      break;

    //ALU instructions
    case ADDI:
      //if there is space in the reservation then issue the instruction
      if(aluReservationStation->freeSpace()) {
        //get the destination architectural register
        int architecturalRegister = currentInstruction.operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Add instruction to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstruction);

        //rename the source operands
        currentInstruction.operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[1]);

         //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstruction.operands[0], newPhysicalRegister);

        //rename destination operand
        currentInstruction.operands[0] = newPhysicalRegister;

        //unblock the pipeline
        blockingFlag = false;
      }
      //otherwise block the pipeline
      else {
        blockingFlag = true;
      }
      break;

    case ADD:
    case AND:
    case MULT:
    case OR:
    case SUB:

      //if there is space in the reservation then issue the instruction
      if(aluReservationStation->freeSpace()) {
        //get the destination architectural register
        int architecturalRegister = currentInstruction.operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Add instruction to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstruction);

        //rename the source operands
        currentInstruction.operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[1]);
        currentInstruction.operands[2] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[2]);

        //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstruction.operands[0], newPhysicalRegister);

        //rename the destination operand
        currentInstruction.operands[0] = newPhysicalRegister;

        //unblock the pipeline
        blockingFlag = false;
      }
      //otherwise block the pipeline
      else {
        blockingFlag = true;
      }
      break;

    //Load Store unit instructions
    case LW:
      if(loadStoreUnitReservationStation->freeSpace()) {
        //get the destination architectural register
        int architecturalRegister = currentInstruction.operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstruction);
        
        //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstruction.operands[0], newPhysicalRegister);

        //rename the destination operand
        currentInstruction.operands[0] = newPhysicalRegister;

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
      }
      break;

    case LWR:
      if(loadStoreUnitReservationStation->freeSpace()) {
        //get the destination architectural register
        int architecturalRegister = currentInstruction.operands[0];

        //get the current mapping of the destination architectural register
        int oldPhysicalRegister = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);

        //find a free register to use as the destination
        int newPhysicalRegister = registerFile->findFreePhysicalRegister();
        registerFile->usePhysicalRegister(newPhysicalRegister);

        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, architecturalRegister, newPhysicalRegister, 
          oldPhysicalRegister, currentInstruction);

        //rename the source operands
        currentInstruction.operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[1]);

        //update the rename table
        registerFile->setArchitecturalRegisterMapping(currentInstruction.operands[0], newPhysicalRegister);

        //rename destination operand
        currentInstruction.operands[0] = newPhysicalRegister;

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
      }
      break;

    case SW:
      if(loadStoreUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_MEMORY, 0, 0, 0, 0, currentInstruction);

        //rename the registers
        currentInstruction.operands[0] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
      }
      break;

    case SWR:
      if(loadStoreUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_MEMORY, 0, 0, 0, 0, currentInstruction);

        //rename the registers
        currentInstruction.operands[0] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);
        currentInstruction.operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[1]);

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
      }
      break;

    //Branch unit instructions
    case BEQ:
    case BNE:
      if(branchUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[2], 0, 0, 0, currentInstruction);

        //rename the registers
        currentInstruction.operands[0] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[0]);
        currentInstruction.operands[1] = registerFile->getArchitecturalRegisterMapping(currentInstruction.operands[1]);

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
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
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[0], 0, 0, 0, currentInstruction);

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
      }
      break;
                    
    //Instruction to finish the program
    case HALT:
      if(branchUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(SYSCALL, 0, 0, 0, 0, currentInstruction);

        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        //block the pipeline
        blockingFlag = true;
      }
      break;
  }
}

void DecodeIssueUnit::pipe() {
  if(!blockingFlag) {
  //send the current instruction to the necessary component
  switch(currentInstruction.opcode) {

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
      registerFile->setScoreBoardValue(currentInstruction.operands[0],0);
      aluReservationStation->setNextInstruction(currentInstruction);
      aluReservationStation->setNextReorderBufferIndex(reorderBufferIndex);
      break;

    //Load Store unit instructions
    case LW:
    case LWR:
      //Set the scoreboard value of the destination register to zero
      registerFile->setScoreBoardValue(currentInstruction.operands[0],0);
    case SW:
    case SWR:
      loadStoreUnitReservationStation->setNextInstruction(currentInstruction);
      loadStoreUnitReservationStation->setNextReorderBufferIndex(reorderBufferIndex);
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
      branchUnitReservationStation->setNextInstruction(currentInstruction);
      branchUnitReservationStation->setNextReorderBufferIndex(reorderBufferIndex);
      break;
  }
  //set the current instruction equal to the next instruction
  currentInstruction = nextInstruction;
  //clear the nextInstruction
  nextInstruction = (Instruction) {0,0,0,0};
}
}

void DecodeIssueUnit::print() const {
  printf("DECODE ISSUE UNIT:");
  printInstruction(currentInstruction);
}

void DecodeIssueUnit::setNextInstruction(const Instruction x) {
  nextInstruction = x;
}

void DecodeIssueUnit::flush() {
  nextInstruction = (Instruction) {0,0,0,0};
  currentInstruction = (Instruction) {0,0,0,0};
  reorderBufferIndex = -1;
  blockingFlag = false;
}

Instruction DecodeIssueUnit::getCurrentInstruction() const {
  return currentInstruction;
}

bool DecodeIssueUnit::getBlockingFlag() const {
  return blockingFlag;
}