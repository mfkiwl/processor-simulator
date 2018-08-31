//===========================================
//included header file containing interface
#include "model.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <string>

//===========================================================================================================
//public functions

Model::Model(const Instructions instructions) :

  //instructions to execute
  instructions(instructions),

  //general stats
  noOfInstructionsExecuted(0),
  noOfClockCycles(0),
  noOfInstructionsPerCycle(0),

  //processor configuration
  numArchitecturalRegisters(16),
  numPhysicalRegisters(64),
  memorySize(28),
  issueWindowSize(4),
  aluReservationStationSize(4),
  branchUnitReservationStationSize(4),
  storeQueueSize(4),
  loadQueueSize(4),
  reorderBufferSize(22),
  numALUs(2),

  //special purpose registers
  pc(0),

  //status flags
  branchPrediction(true),
  runningFlag(true),

  //components
  registerFile(numArchitecturalRegisters, numPhysicalRegisters), 
  memory(memorySize),
  reorderBuffer(&registerFile, &memory, &fetchUnit, &pc, &runningFlag, &noOfInstructionsExecuted, reorderBufferSize, 
    issueWindowSize),
  fetchUnit(instructions, &pc, &decodeIssueUnit, issueWindowSize, branchPrediction),
  decodeIssueUnit(&registerFile, &reorderBuffer, &aluReservationStation, &branchUnitReservationStation, 
    &storeQueue, &loadQueue, issueWindowSize, branchPrediction),
  alu(new ALU[numALUs]),
  aluReservationStation(&registerFile, numALUs, alu, aluReservationStationSize),
  branchUnit(&reorderBuffer),
  branchUnitReservationStation(&registerFile, &branchUnit, branchUnitReservationStationSize),
  loadStoreUnit(&memory, &reorderBuffer, &aluReservationStation),
  storeQueue(&reorderBuffer, &registerFile, &loadStoreUnit, storeQueueSize),
  loadQueue(&reorderBuffer, &registerFile, &storeQueue, &loadStoreUnit, loadQueueSize)
{
  for(int i = 0; i < numALUs; i++) {
    alu[i].setReorderBufferPointer(&reorderBuffer);
    alu[i].setALUReservationStationPointer(&aluReservationStation);
    alu[i].setStoreQueuePointer(&storeQueue);
    alu[i].setLoadQueuePointer(&loadQueue);
    alu[i].setBranchUnitReservationStationPointer(&branchUnitReservationStation);
  }
}

void Model::cycle() {

  //pipeline the instructions
  pipe();

  //writeback the results
  commit();
  //check if we should flush the pipeline
  if(reorderBuffer.getFlushFlag() == 1) {
    flushPipeline();
  }

  //execute the instruction
  execute();

  //dispatch instructions from the reservation stations
  dispatch();

  //decode the instruction
  decodeIssue();

  //fetch the next instruction
  fetch();

  //update the processor stats
  updateStats();

}

void Model::printInfo() const {
  printf("______________________________________\n\n");
  printf("Number of instructions executed: %d\n", noOfInstructionsExecuted);
  printf("Number of clock cycles: %d\n", noOfClockCycles);
  printf("Instruction per cycle: %.2f\n", noOfInstructionsPerCycle);
  printf("\n");
  printf("PC: %d\n", pc);
  registerFile.printRegisters();
}

//=================================================================================================
//private functions

void Model::run() {

  char str[3];
  printf("Keep pressing ENTER to step through the program\n");

  printInfo();

  //step through the program
  while(runningFlag) {

    //hold up the program at each clock cycle
    if(str[0] != 'e') {
      fgets(str, 2, stdin);
    }
    
    //perform one clock cycle
    cycle();
  }

  printf("PROGRAM FINISHED\n");
}

void Model::fetch() {
  fetchUnit.execute();
}

void Model::decodeIssue() {
  decodeIssueUnit.execute();
}

void Model::dispatch() {
  aluReservationStation.execute();
  branchUnitReservationStation.execute();
  storeQueue.execute();
  loadQueue.execute();
}

void Model::execute() {
  for(int i = 0; i < numALUs; i++) {
    alu[i].execute();
  }
  loadStoreUnit.execute();
  branchUnit.execute();
}

void Model::commit() {
  reorderBuffer.execute();
}

void Model::pipe() {
  //propogate outputs of the decode/issue unit and the fetch unit through pipeline
  fetchUnit.pipe();
  decodeIssueUnit.pipe();

  //propogate the outputs of the reservation stations through the pipeline
  aluReservationStation.pipe();
  branchUnitReservationStation.pipe();
  storeQueue.pipe();
  loadQueue.pipe();

  //propogate the outputs of the reservation stations through the pipeline
  for(int i = 0; i < numALUs; i++) {
    alu[i].pipe();
  }
  branchUnit.pipe();
  loadStoreUnit.pipe();
}

void Model::flushPipeline() {
  //flush register file
  registerFile.flush();
  //flush decode unit
  decodeIssueUnit.flush();
  //flush reservation stations
  aluReservationStation.flush();
  branchUnitReservationStation.flush();
  storeQueue.flush();
  loadQueue.flush();
  //flush execution units
  for(int i = 0; i < numALUs; i++) {
    alu[i].flush();
  }
  branchUnit.flush();
  loadStoreUnit.flush();
  //flush reorder buffer
  reorderBuffer.flush();
  //reset the register file scoreboard
  registerFile.resetScoreBoard();
}

void Model::updateStats() {
  //increment the number of clock cycles performed
  noOfClockCycles++;
  //calculate the number of instructions exected per clock cycles
  if(noOfClockCycles == 0) {
    noOfInstructionsPerCycle = 0;
  }
  else {
    noOfInstructionsPerCycle = (float) noOfInstructionsExecuted / (float) noOfClockCycles;
  }
}

//========================================================================================================
// getter and setter functions

int Model::getNumRegisters() const {
  return numArchitecturalRegisters;
}

int Model::getMemorySize() const {
  return memorySize;
}

int Model::getIssueWindowSize() const {
  return issueWindowSize;
}

bool Model::getRunningFlag() const {
  return runningFlag;
}

int Model::getNoOfInstructionsExecuted() const {
  return noOfInstructionsExecuted;
}

int Model::getNoOfClockCycles() const {
  return noOfClockCycles;
}

float Model::getNoOfInstructionsExecutedPerCycle() const {
  return noOfInstructionsPerCycle;
}

int Model::getPC() const {
  return pc;
}

void Model::getRenameTable(int* const copy) const {
  registerFile.getRenameTable(copy);
}

void Model::getArchitecturalRegisterValues(int* const copy) const {
  registerFile.getArchitecturalRegisterValues(copy);
}

void Model::getLatestArchitecturalRegisterValues(int* const copy) const {
  registerFile.getLatestArchitecturalRegisterValues(copy);
}

void Model::getAllMemoryValues(int* const copy) const {
  memory.getAllMemoryValues(copy);
}

void Model::getFetchUnitInstructions(Instruction* const copy) const {
  fetchUnit.getInstructions(copy);
}

void Model::getDecodeIssueUnitInstructions(Instruction* const copy) const {
  decodeIssueUnit.getInstructions(copy);
}

void Model::getDecodeIssueUnitReorderBufferIndexes(int* const copy) const {
  decodeIssueUnit.getReorderBufferIndexes(copy);
}

int Model::getAluReservationStationSize() const {
  return aluReservationStationSize;
}

void Model::getAluReservationStationInstructions(Instruction* const copy) const {
  aluReservationStation.getCurrentInstructions(copy);
}

void Model::getAluReservationStationReorderBufferIndexes(int* const copy) const {
  aluReservationStation.getCurrentReorderBufferIndexes(copy);
}

void Model::getAluReservationStationOperandTypes(OperandType** const copy) const {
  aluReservationStation.getOperandTypes(copy);
}

int Model::getBranchUnitReservationStationSize() const {
  return branchUnitReservationStationSize;
}

void Model::getBranchUnitReservationStationInstructions(Instruction* const copy) const {
  branchUnitReservationStation.getCurrentInstructions(copy);
}

void Model::getBranchUnitReservationStationReorderBufferIndexes(int* const copy) const {
  branchUnitReservationStation.getCurrentReorderBufferIndexes(copy);
}

void Model::getBranchUnitReservationStationValidBits(bool** const copy) const {
  branchUnitReservationStation.getValidBits(copy);
}

int Model::getStoreQueueSize() const {
  return storeQueueSize;
}

void Model::getStoreQueueInstructions(Instruction* const copy) const {
  storeQueue.getCurrentInstructions(copy);
}

void Model::getStoreQueueReorderBufferIndexes(int* const copy) const {
  storeQueue.getCurrentReorderBufferIndexes(copy);
}

void Model::getStoreQueueValidBits(bool** const copy) const {
  storeQueue.getValidBits(copy);
}

int Model::getLoadQueueSize() const {
  return loadQueueSize;
}

void Model::getLoadQueueInstructions(Instruction* const copy) const {
  loadQueue.getCurrentInstructions(copy);
}

void Model::getLoadQueueReorderBufferIndexes(int* const copy) const {
  loadQueue.getCurrentReorderBufferIndexes(copy);
}

void Model::getLoadQueueValidBits(bool** const copy) const {
  loadQueue.getValidBits(copy);
}

int Model::getNumALUs() const {
  return numALUs;
}

void Model::getAluResults(int* const copy) const {
  for(int i = 0; i < numALUs; i++) {
    copy[i] = alu[i].getResult();
  }
}

int Model::getReorderBufferSize() const {
  return reorderBufferSize;
}

int Model::getNumReorderBufferFields() const {
  return reorderBuffer.getNumFields();
}

void Model::getAluReorderBufferIndexes(int* const copy) const {
  for(int i = 0; i < numALUs; i++) {
    copy[i] = alu[i].getReorderBufferIndex();
  }
}

bool Model::getBranchUnitSuccessful() const {
  return branchUnit.getSuccessful();
}

int Model::getBranchUnitReorderBufferIndex() const {
  return branchUnit.getReorderBufferIndex();
}

void Model::getReorderBufferInstructions(Instruction* const copy) const {
  reorderBuffer.getReorderBufferInstructions(copy);
}

void Model::getReorderBufferFields(int** const copy) const {
  reorderBuffer.getReorderBufferFields(copy);
}

int Model::getReorderBufferTailIndex() const {
  return reorderBuffer.getTailIndex();
}

int Model::getReorderBufferHeadIndex() const {
  return reorderBuffer.getHeadIndex();
}