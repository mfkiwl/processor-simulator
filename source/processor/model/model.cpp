//===========================================
//included header file containing interface
#include "model.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <string>

//===========================================
//class implementation

Model::Model(const Instructions instructions, const int numOfRegisters, const int memorySize, 
  const int aluReservationStationSize, const int branchUnitReservationStationSize, 
  const int loadStoreUnitReservationStationSize, const int reorderBufferSize) : 

  //processor configuration
  numOfRegisters(numOfRegisters),
  memorySize(memorySize),
  aluReservationStationSize(aluReservationStationSize),
  branchUnitReservationStationSize(branchUnitReservationStationSize),
  loadStoreUnitReservationStationSize(loadStoreUnitReservationStationSize),
  reorderBufferSize(reorderBufferSize),

  //general stats
  noOfInstructionsExecuted(0),
  noOfClockCycles(0),
  noOfInstructionsPerCycle(0),

  //instruction info
  instructions(instructions),

  //registers
  pc(1),

  //status flags
  runningFlag(1),

  //components
  registerFile(numOfRegisters), 
  memory(memorySize),
  reorderBuffer(&registerFile, &memory, &pc, &runningFlag, &noOfInstructionsExecuted, reorderBufferSize),
  fetchUnit(instructions, &pc, &decodeIssueUnit),
  decodeIssueUnit(&registerFile, &reorderBuffer, &aluReservationStation, &branchUnitReservationStation, &loadStoreUnitReservationStation),
  alu(&reorderBuffer),
  aluReservationStation(&registerFile, &alu, aluReservationStationSize),
  branchUnit(&reorderBuffer),
  branchUnitReservationStation(&registerFile, &branchUnit, branchUnitReservationStationSize),
  loadStoreUnit(&memory, &reorderBuffer),
  loadStoreUnitReservationStation(&registerFile, &reorderBuffer, &loadStoreUnit, loadStoreUnitReservationStationSize)
{}

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
  fetch(decodeIssueUnit.getBlockingFlag());

  //update the processor stats
  updateStats();

  //print processor current information
  printInfo();
}

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

void Model::fetch(bool blocking) {
  fetchUnit.execute(blocking);
}

void Model::decodeIssue() {
  decodeIssueUnit.execute();
}

void Model::dispatch() {
  aluReservationStation.execute();
  branchUnitReservationStation.execute();
  loadStoreUnitReservationStation.execute();
}

void Model::execute() {
  alu.execute();
  loadStoreUnit.execute();
  branchUnit.execute();
}

void Model::commit() {
  reorderBuffer.retire();
}

void Model::pipe() {
  //propogate outputs of the decode/issue unit and the fetch unit through pipeline
  fetchUnit.pipe(decodeIssueUnit.getBlockingFlag());
  decodeIssueUnit.pipe();

  //propogate the outputs of the reservation stations through the pipeline
  aluReservationStation.pipe();
  branchUnitReservationStation.pipe();
  loadStoreUnitReservationStation.pipe();

  //propogate the outputs of the reservation stations through the pipeline
  alu.pipe();
  branchUnit.pipe();
  loadStoreUnit.pipe();
}

void Model::flushPipeline() {
  //flush decode unit
  decodeIssueUnit.flush();
  //flush reservation stations
  aluReservationStation.flush();
  branchUnitReservationStation.flush();
  loadStoreUnitReservationStation.flush();
  //flush execution units
  alu.flush();
  branchUnit.flush();
  loadStoreUnit.flush();
  //flush reorder buffer
  reorderBuffer.flush();
  //reset the register file scoreboard
  registerFile.resetScoreBoard();

  printf("FLUSHING PIPELINE!\n");
}

void Model::printInfo() const {
  printf("______________________________________\n\n");
  printf("Number of instructions executed: %d\n", noOfInstructionsExecuted);
  printf("Number of clock cycles: %d\n", noOfClockCycles);
  printf("Instruction per cycle: %.2f\n", noOfInstructionsPerCycle);
  printf("\n");
  printf("PC: %d\n", pc);
  registerFile.printRegisters();
  printf("\n");
  aluReservationStation.print();
  printf("\n");
  branchUnitReservationStation.print();
  printf("\n");
  loadStoreUnitReservationStation.print();
  printf("\n");
  reorderBuffer.print();
}

//=============================================
// getter functions

int Model::getNumOfRegisters() const {
  return numOfRegisters;
}

int Model::getMemorySize() const {
  return memorySize;
}

int Model::getRunningFlag() const {
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

void Model::getAllRegisterValues(int* const copy) const {
  registerFile.getAllRegisterValues(copy);
}

void Model::getScoreBoard(int* const copy) const {
  registerFile.getScoreBoard(copy);
}

void Model::getAllMemoryValues(int* const copy) const {
  memory.getAllMemoryValues(copy);
}

Instruction Model::getFetchUnitInstruction() const {
  return fetchUnit.getCurrentInstruction();
}

Instruction Model::getDecodeIssueUnitInstruction() const {
  return decodeIssueUnit.getCurrentInstruction();
}

void Model::getAluReservationStationInstructions(Instruction* const copy) const {
  aluReservationStation.getCurrentInstructions(copy);
}

void Model::getAluReservationStationReorderBufferIndexes(int* const copy) const {
  aluReservationStation.getCurrentReorderBufferIndexes(copy);
}

void Model::getBranchUnitReservationStationInstructions(Instruction* const copy) const {
  branchUnitReservationStation.getCurrentInstructions(copy);
}

void Model::getBranchUnitReservationStationReorderBufferIndexes(int* const copy) const {
  branchUnitReservationStation.getCurrentReorderBufferIndexes(copy);
}

void Model::getLoadStoreUnitReservationStationInstructions(Instruction* const copy) const {
  loadStoreUnitReservationStation.getCurrentInstructions(copy);
}

void Model::getLoadStoreUnitReservationStationReorderBufferIndexes(int* const copy) const {
  loadStoreUnitReservationStation.getCurrentReorderBufferIndexes(copy);
}

int Model::getAluResult() const {
  return alu.getResult();
}

int Model::getAluReorderBufferIndex() const {
  return alu.getReorderBufferIndex();
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