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
  issue();
}

void DecodeIssueUnit::registerRename() {
  
}

void DecodeIssueUnit::issue() {
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
      //if there is space in the reservation then issue the instruction
      if(aluReservationStation->freeSpace() && registerFile->getScoreBoardValue(currentInstruction.operands[0])) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, currentInstruction.operands[0], 0, currentInstruction);
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
    case LWR:
      if(loadStoreUnitReservationStation->freeSpace() && registerFile->getScoreBoardValue(currentInstruction.operands[0])) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, 0, currentInstruction.operands[0], 0, currentInstruction);
        //unblock the pipeline
        blockingFlag = false;
      }
      else {
        blockingFlag = true;
      }
      break;
    case SW:
    case SWR:
      if(loadStoreUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_MEMORY, 0, currentInstruction.operands[1], 0, currentInstruction);
        blockingFlag = false;
      }
      else {
        blockingFlag = true;
      }
      break;

    //Branch unit instructions
    case BEQ:
    case BNE:
      if(branchUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[2], 0, 0, currentInstruction);
        blockingFlag = false;
      }
      else {
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
        reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[0], 0, 0, currentInstruction);
        blockingFlag = false;
      }
      else {
        blockingFlag = true;
      }
      break;
                    
    //Instruction to finish the program
    case HALT:
      if(branchUnitReservationStation->freeSpace()) {
        //Instruction has been issued so add entry to the reorder buffer
        reorderBufferIndex = reorderBuffer->addEntry(SYSCALL, 0, 0, 0, currentInstruction);
        blockingFlag = false;
      }
      else {
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