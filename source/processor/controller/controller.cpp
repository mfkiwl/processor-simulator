//===========================================
//included header file containing interface
#include "controller.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "../model/model.h"
#include "../view/view.h"
#include "../model/constants.h"

//===========================================
// class implementation

Controller::Controller(const Instructions instructions) :
  model(instructions),
  view()
{}

//Render the current frame with the appropriate information from the processor
void Controller::updateView() {

  //handle events
  view.eventHandler();

  //Clear the screen
  view.clearScreen();

  //draw all the components
  drawStats();
  drawPC();
  view.drawInstructions();
  drawFetchUnit();
  drawDecodeIssueUnit();
  drawAluReservationStation();
  drawBranchUnitReservationStation();
  drawStoreQueue();
  drawLoadQueue();
  drawAlu();
  view.drawLoadStoreUnit();
  drawBranchUnit();
  drawReorderBuffer();
  drawRegisterFile();
  drawMemory();
  view.drawConnections();

  //Update  the screen
  view.updateScreen();
}


int Controller::start(const Instructions instructions) {

  //Start up SDL and create window
  if( !view.init() )
  {
    printf( "Failed to initialize!\n" );
    //Free resources and close SDL
    view.close();
    return 1;
  }
  
  //Load SDL media
  if( !view.loadMedia() )
  {
    printf( "Failed to load media!\n" );
    //Free resources and close SDL
    view.close();
    return 1;
  }

  //buffer to hold the user input
  char str[3];

  //message to display in the terminal
  printf("Keep pressing ENTER to step through the program\n");

  //display initial information
  //model.printInfo();
  updateView();

  //While application is running
  while( !view.hasQuit() && model.getRunningFlag())
  {

    //hold up the program at each clock cycle
    if(str[0] != 'e') {
      fgets(str, 2, stdin);
    }
    
    //perform one clock cycle
    model.cycle();
    
    //update the display
    updateView();
  }

  printf("PROGRAM FINISHED\n");

  //print the final state of the processor
  model.printInfo();

  //Free resources and close SDL
  view.close();

  return 0;
}

//===========================================================================================================
//private functions
void Controller::drawStats() {
  //render the processor stats
  int numOfInstructionsExecuted = model.getNoOfInstructionsExecuted();
  int numOfClockCycles = model.getNoOfClockCycles();
  float numOfInstructionsExecutedPerCycle = model.getNoOfInstructionsExecutedPerCycle();
  view.drawProcessorStats(numOfInstructionsExecuted, numOfClockCycles, numOfInstructionsExecutedPerCycle);
}

void Controller::drawPC() {
  //draw the program counter
  int programCounter = model.getPC();
  view.drawPC(programCounter);
}

void Controller::drawFetchUnit() {
  //draw the instruction in the fetch unit
  int issueWindowSize = model.getIssueWindowSize();
  Instruction fetchUnitInstructions[issueWindowSize];
  model.getFetchUnitInstructions(fetchUnitInstructions);
  view.drawFetchUnit(issueWindowSize, fetchUnitInstructions);
}

void Controller::drawDecodeIssueUnit() {
  //draw the instruction in the decode issue unit
  int issueWindowSize = model.getIssueWindowSize();
  Instruction decodeIssueUnitInstructions[issueWindowSize];
  int decodeIssueUnitReorderBufferIndexes[issueWindowSize];
  OperandType decodeIssueUnitOperandTypes[issueWindowSize][numOfOperands];
  model.getDecodeIssueUnitInstructions(decodeIssueUnitInstructions);
  model.getDecodeIssueUnitReorderBufferIndexes(decodeIssueUnitReorderBufferIndexes);
  model.getDecodeIssueUnitOperandTypes(decodeIssueUnitOperandTypes);
  view.drawDecodeIssueUnit(issueWindowSize, decodeIssueUnitInstructions, decodeIssueUnitReorderBufferIndexes,
    decodeIssueUnitOperandTypes);
}

void Controller::drawAluReservationStation() {
  //draw the instructions in the alu reservation station
  int aluReservationStationSize = model.getAluReservationStationSize();
  Instruction aluReservationStationInstructions[aluReservationStationSize];
  model.getAluReservationStationInstructions(aluReservationStationInstructions);
  int aluReservationStationReorderBufferIndexes[aluReservationStationSize];
  model.getAluReservationStationReorderBufferIndexes(aluReservationStationReorderBufferIndexes);
  OperandType aluReservationStationOperandTypes[aluReservationStationSize][numOfOperands];
  model.getAluReservationStationOperandTypes(aluReservationStationOperandTypes);
  view.drawAluReservationStation(aluReservationStationSize, aluReservationStationInstructions, 
    aluReservationStationReorderBufferIndexes, aluReservationStationOperandTypes);
}

void Controller::drawBranchUnitReservationStation() {
  //draw the instructions in the branch unit reservation station
  int branchUnitReservationStationSize = model.getBranchUnitReservationStationSize();
  Instruction branchUnitReservationStationInstructions[branchUnitReservationStationSize];
  model.getBranchUnitReservationStationInstructions(branchUnitReservationStationInstructions);
  int branchUnitReservationStationReorderBufferIndexes[branchUnitReservationStationSize];
  model.getBranchUnitReservationStationReorderBufferIndexes(branchUnitReservationStationReorderBufferIndexes);
  OperandType branchUnitReservationStationOperandTypes[branchUnitReservationStationSize][numOfOperands];
  model.getBranchUnitReservationStationOperandTypes(branchUnitReservationStationOperandTypes);
  view.drawBranchUnitReservationStation(branchUnitReservationStationSize, branchUnitReservationStationInstructions, 
    branchUnitReservationStationReorderBufferIndexes, branchUnitReservationStationOperandTypes);
}

void Controller::drawStoreQueue() {
  //draw the instructions in the store queue
  int storeQueueSize = model.getStoreQueueSize();
  Instruction storeQueueInstructions[storeQueueSize];
  model.getStoreQueueInstructions(storeQueueInstructions);
  int storeQueueReorderBufferIndexes[storeQueueSize];
  model.getStoreQueueReorderBufferIndexes(storeQueueReorderBufferIndexes);
  OperandType storeQueueOperandTypes[storeQueueSize][numOfOperands];
  model.getStoreQueueOperandTypes(storeQueueOperandTypes);
  view.drawStoreQueue(storeQueueSize, storeQueueInstructions, storeQueueReorderBufferIndexes, storeQueueOperandTypes);
}

void Controller::drawLoadQueue() {
  //draw the instructions in the load queue
  int loadQueueSize = model.getLoadQueueSize();
  Instruction loadQueueInstructions[loadQueueSize];
  model.getLoadQueueInstructions(loadQueueInstructions);
  int loadQueueReorderBufferIndexes[loadQueueSize];
  model.getLoadQueueReorderBufferIndexes(loadQueueReorderBufferIndexes);
  OperandType loadQueueOperandTypes[loadQueueSize][numOfOperands];
  model.getLoadQueueOperandTypes(loadQueueOperandTypes);
  view.drawLoadQueue(loadQueueSize, loadQueueInstructions, loadQueueReorderBufferIndexes, loadQueueOperandTypes);
}

void Controller::drawAlu() {
  //draw the alu
  int numALUs = model.getNumALUs();
  int aluResults[numALUs];
  model.getAluResults(aluResults);
  int aluReorderBufferIndexes[numALUs];
  model.getAluReorderBufferIndexes(aluReorderBufferIndexes);
  view.drawAlu(numALUs, aluResults, aluReorderBufferIndexes);
}

void Controller::drawBranchUnit() {
  //draw the branch unit
  bool branchUnitSuccessful = model.getBranchUnitSuccessful();
  int branchUnitReorderBufferIndex = model.getBranchUnitReorderBufferIndex();
  view.drawBranchUnit(branchUnitSuccessful, branchUnitReorderBufferIndex);
}

void Controller::drawReorderBuffer() {
  //draw the reorder buffer
  int reorderBufferSize = model.getReorderBufferSize();
  int reorderBufferTailIndex = model.getReorderBufferTailIndex();
  int reorderBufferHeadIndex = model.getReorderBufferHeadIndex();
  Instruction reorderBufferInstructions[reorderBufferSize];
  model.getReorderBufferInstructions(reorderBufferInstructions);
  int reorderBufferFields[reorderBufferSize][ReorderBufferIndex::COUNT];
  model.getReorderBufferFields(reorderBufferFields);
  view.drawReorderBuffer(reorderBufferSize, reorderBufferTailIndex, reorderBufferHeadIndex, reorderBufferInstructions, 
    reorderBufferFields);
}

void Controller::drawRegisterFile() {
  //draw the register file
  int numRegisters = model.getNumRegisters();
  int registerValues[numRegisters];
  model.getRegisterValues(registerValues);
  int renameTable[numRegisters];
  model.getRenameTable(renameTable);
  bool robMapping[numRegisters];
  model.getRobMapping(robMapping);
  view.drawRegisterFile(numRegisters, registerValues, renameTable, robMapping);
}

void Controller::drawMemory() {
  //draw the memory
  int memorySize = model.getMemorySize();
  int memoryValues[memorySize];
  model.getAllMemoryValues(memoryValues);
  view.drawMemory(memorySize, memoryValues);
}