//===========================================
//included header file containing interface
#include "processor.h"

//===========================================
// included dependencies
#include <iostream>
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
  cout << "Keep pressing ENTER to step through the program\n";

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

  cout << endl << "PROGRAM FINISHED" << endl;
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

  cout << "FLUSHING PIPELINE!\n";
}

void Processor::printInfo() {

  cout << "______________________________________" << endl << endl;

  cout << "Number of clock cycles: " << noOfClockCycles << endl;
  cout << "Number of instructions executed: " << noOfInstructionsExecuted << endl;
  float instructionsPerCycle;
  if(noOfClockCycles == 0) {
    instructionsPerCycle = 0;
  }
  else {
    instructionsPerCycle = (float) noOfInstructionsExecuted / (float) noOfClockCycles;
  }
  cout << "Instruction per cycle: " << instructionsPerCycle << endl;
  cout << endl;
  cout << "PC: " << pc << endl;
  registerFile.printRegisters();
  cout << endl;
  aluReservationStation.print();
  cout << endl;
  branchUnitReservationStation.print();
  cout << endl;
  loadStoreUnitReservationStation.print();
  cout << endl;
  reorderBuffer.print();
}