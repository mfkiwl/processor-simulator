//===========================================
//included header file containing interface
#include "controller.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "../model/model.h"
#include "../view/view.h"

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

  //render the processor stats
  int numOfInstructionsExecuted = model.getNoOfInstructionsExecuted();
  int numOfClockCycles = model.getNoOfClockCycles();
  float numOfInstructionsExecutedPerCycle = model.getNoOfInstructionsExecutedPerCycle();
  view.drawProcessorStats(numOfInstructionsExecuted, numOfClockCycles, numOfInstructionsExecutedPerCycle);

  //draw the program counter
  int programCounter = model.getPC();
  view.drawPC(programCounter);

  int issueWindowSize = model.getIssueWindowSize();
  
  //draw the instruction in the fetch unit
  Instruction fetchUnitInstructions[issueWindowSize];
  model.getFetchUnitInstructions(fetchUnitInstructions);
  view.drawFetchUnit(issueWindowSize, fetchUnitInstructions);

  //draw the instruction in the decode issue unit
  Instruction decodeIssueUnitInstructions[issueWindowSize];
  int decodeIssueUnitReorderBufferIndexes[issueWindowSize];
  model.getDecodeIssueUnitInstructions(decodeIssueUnitInstructions);
  model.getDecodeIssueUnitReorderBufferIndexes(decodeIssueUnitReorderBufferIndexes);
  view.drawDecodeIssueUnit(issueWindowSize, decodeIssueUnitInstructions, decodeIssueUnitReorderBufferIndexes);

  //draw the instructions in the alu reservation station
  int aluReservationStationSize = model.getAluReservationStationSize();
  Instruction aluReservationStationInstructions[aluReservationStationSize];
  model.getAluReservationStationInstructions(aluReservationStationInstructions);
  int aluReservationStationReorderBufferIndexes[aluReservationStationSize];
  model.getAluReservationStationReorderBufferIndexes(aluReservationStationReorderBufferIndexes);
  view.drawAluReservationStation(aluReservationStationSize, aluReservationStationInstructions, 
    aluReservationStationReorderBufferIndexes);

  //draw the instructions in the branch unit reservation station
  int branchUnitReservationStationSize = model.getBranchUnitReservationStationSize();
  Instruction branchUnitReservationStationInstructions[branchUnitReservationStationSize];
  model.getBranchUnitReservationStationInstructions(branchUnitReservationStationInstructions);
  int branchUnitReservationStationReorderBufferIndexes[branchUnitReservationStationSize];
  model.getBranchUnitReservationStationReorderBufferIndexes(branchUnitReservationStationReorderBufferIndexes);
  view.drawBranchUnitReservationStation(branchUnitReservationStationSize, branchUnitReservationStationInstructions, 
    branchUnitReservationStationReorderBufferIndexes);

  //draw the instructions in the store queue
  int storeQueueSize = model.getStoreQueueSize();
  Instruction storeQueueInstructions[storeQueueSize];
  model.getStoreQueueInstructions(storeQueueInstructions);
  int storeQueueReorderBufferIndexes[storeQueueSize];
  model.getStoreQueueReorderBufferIndexes(storeQueueReorderBufferIndexes);
  view.drawStoreQueue(storeQueueSize, storeQueueInstructions, 
    storeQueueReorderBufferIndexes);

  //draw the instructions in the load queue
  int loadQueueSize = model.getLoadQueueSize();
  Instruction loadQueueInstructions[loadQueueSize];
  model.getLoadQueueInstructions(loadQueueInstructions);
  int loadQueueReorderBufferIndexes[loadQueueSize];
  model.getLoadQueueReorderBufferIndexes(loadQueueReorderBufferIndexes);
  view.drawLoadQueue(loadQueueSize, loadQueueInstructions, 
    loadQueueReorderBufferIndexes);

  //draw the alu
  int numALUs = model.getNumALUs();
  int aluResults[numALUs];
  model.getAluResults(aluResults);
  int aluReorderBufferIndexes[numALUs];
  model.getAluReorderBufferIndexes(aluReorderBufferIndexes);
  view.drawAlu(numALUs, aluResults, aluReorderBufferIndexes);

  //draw the branch unit
  bool branchUnitSuccessful = model.getBranchUnitSuccessful();
  int branchUnitReorderBufferIndex = model.getBranchUnitReorderBufferIndex();
  view.drawBranchUnit(branchUnitSuccessful, branchUnitReorderBufferIndex);

  //draw the reorder buffer
  int reorderBufferSize = model.getReorderBufferSize();
  int numReorderBufferFields = model.getNumReorderBufferFields();
  int reorderBufferTailIndex = model.getReorderBufferTailIndex();
  int reorderBufferHeadIndex = model.getReorderBufferHeadIndex();
  Instruction reorderBufferInstructions[reorderBufferSize];
  model.getReorderBufferInstructions(reorderBufferInstructions);
  int** reorderBufferFields = new int*[reorderBufferSize];
  for(int i = 0; i < reorderBufferSize; i++) {
    reorderBufferFields[i] = new int[numReorderBufferFields];
  }
  model.getReorderBufferFields(reorderBufferFields);
  view.drawReorderBuffer(reorderBufferSize, numReorderBufferFields, reorderBufferTailIndex, reorderBufferHeadIndex, reorderBufferInstructions, reorderBufferFields);

  //draw the register file
  int numRegisters = model.getNumRegisters();
  int registerValues[numRegisters];
  model.getLatestArchitecturalRegisterValues(registerValues);
  int renameTable[numRegisters];
  model.getRenameTable(renameTable);
  view.drawRegisterFile(numRegisters, registerValues, renameTable);

  //draw the memory
  int memorySize = model.getMemorySize();
  int memoryValues[memorySize];
  model.getAllMemoryValues(memoryValues);
  view.drawMemory(memorySize, memoryValues);

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