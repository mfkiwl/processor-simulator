//===========================================
//included header file containing interface
#include "processor.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <string>
#include <fstream>

//===========================================
//class implementation

using namespace std;

Processor::Processor(Instructions instructions) : 

  //processor configuration
  memorySize(24),

  //general stats
  noOfInstructionsExecuted(0),
  noOfClockCycles(0),

  //instruction info
  instructions(instructions),

  //registers
  pc(1),

  //status flags
  flushFlag(0),
  runningFlag(1),
  decodeUnitBlockingFlag(0),

  //components
  registerFile(), 
  memory(memorySize),
  reorderBuffer(&registerFile, &memory, &pc, &flushFlag, &runningFlag, &noOfInstructionsExecuted),
  fetchUnit(instructions, &pc, &decodeIssueUnit),
  decodeIssueUnit(&registerFile, &reorderBuffer, &aluReservationStation, &branchUnitReservationStation, &loadStoreUnitReservationStation, &decodeUnitBlockingFlag),
  alu(&registerFile, &reorderBuffer),
  aluReservationStation(&registerFile, &alu),
  branchUnit(&reorderBuffer),
  branchUnitReservationStation(&registerFile, &branchUnit),
  loadStoreUnit(&memory, &reorderBuffer),
  loadStoreUnitReservationStation(&registerFile, &reorderBuffer, &loadStoreUnit)
{}

void Processor::start() {

  char str[3];
  printf("Keep pressing ENTER to step through the program\n");

  printInfo();

  //step through the program
  while(runningFlag) {

    //hold up the program at each clock cycle
    if(str[0] != 'e') {
      fgets(str, 2, stdin);
    }

    //writeback the results
    commit();

    //check if we should flush the pipeline
    if(flushFlag == 1) {
      flushPipeline();
    }

    //execute the instruction
    execute();

    //dispatch instructions from the reservation stations
    dispatch();

    //decode the instruction
    decodeIssue();

    //if the pipeline is not being blocked
    if(!decodeUnitBlockingFlag) {
      //fetch the next instruction
      fetch();
      //propogate outputs of the decode/issue unit and the fetch unit through pipeline
      decodeIssueUnit.pipe();
      fetchUnit.pipe();
    }

    //propogate the outputs of the reservation stations through the pipeline
    alu.pipe();
    branchUnit.pipe();
    loadStoreUnit.pipe();

    //propogate the outputs of the reservation stations through the pipeline
    aluReservationStation.pipe();
    branchUnitReservationStation.pipe();
    loadStoreUnitReservationStation.pipe();

    //update info
    noOfClockCycles++;

    //print register info
    printInfo();
  }

  printf("PROGRAM FINISHED\n");
}

void Processor::fetch() {
  fetchUnit.execute();
}

void Processor::decodeIssue() {
  decodeIssueUnit.execute();
}

void Processor::dispatch() {
  aluReservationStation.execute();
  branchUnitReservationStation.execute();
  loadStoreUnitReservationStation.execute();
}

void Processor::execute() {
  alu.execute();
  loadStoreUnit.execute();
  branchUnit.execute();
}

void Processor::commit() {
  reorderBuffer.retire();
}

void Processor::flushPipeline() {
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
  //reset the flush flag
  flushFlag = 0;
  decodeUnitBlockingFlag = 0;

  printf("FLUSHING PIPELINE!\n");
}

void Processor::printInfo() {

  printf("______________________________________\n\n");

  printf("Number of clock cycles: %d\n", noOfClockCycles);
  printf("Number of instructions executed: %d\n", noOfInstructionsExecuted);
  float instructionsPerCycle;
  if(noOfClockCycles == 0) {
    instructionsPerCycle = 0;
  }
  else {
    instructionsPerCycle = (float) noOfInstructionsExecuted / (float) noOfClockCycles;
  }
  printf("Instruction per cycle: %.2f\n", instructionsPerCycle);
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

int processorMain(int argc, char *argv[]) {
  printf("\n");

  //If command line arguments are incorrect then stop program
  if(argc != 2) {
    printf("Machine code file not valid.\n");
    return 1;
  }

  //get the instructions from the machine code file
  std::string inputFileName(argv[1]);
  Instructions instructions(inputFileName);

  //create processor object and start processing
  if(instructions.getNumOfInstructions() != -1 && instructions.getInstructions() != NULL) {
    Processor processor(instructions);
    processor.start();
  }
  printf("\n");
  return 0;
}
