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

Controller::Controller(const Instructions instructions, const int numOfRegisters, const int memorySize, 
  const int aluReservationStationSize, const int branchUnitReservationStationSize, 
  const int loadStoreUnitReservationStationSize, const int reorderBufferSize) :
  numOfRegisters(numOfRegisters),
  memorySize(memorySize),
  aluReservationStationSize(aluReservationStationSize),
  branchUnitReservationStationSize(branchUnitReservationStationSize),
  loadStoreUnitReservationStationSize(loadStoreUnitReservationStationSize),
  reorderBufferSize(reorderBufferSize),
  model(instructions, numOfRegisters, memorySize, aluReservationStationSize, 
    branchUnitReservationStationSize, loadStoreUnitReservationStationSize, reorderBufferSize),
  view(numOfRegisters, memorySize, aluReservationStationSize, branchUnitReservationStationSize,
    loadStoreUnitReservationStationSize, reorderBufferSize)
{}

//the original main function for the processor model
int Controller::modelMain(const Instructions instructions) {
  
  //running the processor
  char str[3];
  printf("Keep pressing ENTER to step through the program\n");

  model.printInfo();

  //step through the program
  while(model.getRunningFlag()) {

    //hold up the program at each clock cycle
    if(str[0] != 'e') {
      fgets(str, 2, stdin);
    }
    
    //perform one clock cycle
    model.cycle();
  }

  printf("PROGRAM FINISHED\n");
  return 0;
}

//the original main function for the view
int Controller::viewmain(const int argc, const char *argv[])
{

	//Start up SDL and create window
	if( !view.init() )
	{
		printf( "Failed to initialize!\n" );
		//Free resources and close SDL
		view.close();
		return 1;
	}
	
	//Load media
	if( !view.loadMedia() )
	{
    printf( "Failed to load media!\n" );
    //Free resources and close SDL
		view.close();
		return 1;
	}

	//While application is running
	while( !view.hasQuit() )
	{
        //view.frame();
	}

	//Free resources and close SDL
	view.close();

	return 0;
}

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
  
  //draw the instruction in the fetch unit
  Instruction fetchUnitInstruction = model.getFetchUnitInstruction();
  view.drawFetchUnit(fetchUnitInstruction);

  //draw the instruction in the decode issue unit
  Instruction decodeIssueUnitInstruction = model.getDecodeIssueUnitInstruction();
  view.drawDecodeIssueUnit(decodeIssueUnitInstruction);

  //draw the instructions in the alu reservation station
  Instruction aluReservationStationInstructions[aluReservationStationSize];
  model.getAluReservationStationInstructions(aluReservationStationInstructions);
  int aluReservationStationReorderBufferIndexes[4];
  model.getAluReservationStationReorderBufferIndexes(aluReservationStationReorderBufferIndexes);
  view.drawAluReservationStation(aluReservationStationInstructions, aluReservationStationReorderBufferIndexes);

  //draw the instructions in the branch unit reservation station
  Instruction branchUnitReservationStationInstructions[branchUnitReservationStationSize];
  model.getBranchUnitReservationStationInstructions(branchUnitReservationStationInstructions);
  int branchUnitReservationStationReorderBufferIndexes[4];
  model.getBranchUnitReservationStationReorderBufferIndexes(branchUnitReservationStationReorderBufferIndexes);
  view.drawBranchUnitReservationStation(branchUnitReservationStationInstructions, branchUnitReservationStationReorderBufferIndexes);

  //draw the instructions in the load store unit reservation station
  Instruction loadStoreUnitReservationStationInstructions[loadStoreUnitReservationStationSize];
  model.getLoadStoreUnitReservationStationInstructions(loadStoreUnitReservationStationInstructions);
  int loadStoreUnitReservationStationReorderBufferIndexes[4];
  model.getLoadStoreUnitReservationStationReorderBufferIndexes(loadStoreUnitReservationStationReorderBufferIndexes);
  view.drawLoadStoreUnitReservationStation(loadStoreUnitReservationStationInstructions, loadStoreUnitReservationStationReorderBufferIndexes);

  //draw the alu
  int aluResult = model.getAluResult();
  int aluReorderBufferIndex = model.getAluReorderBufferIndex();
  view.drawAlu(aluResult, aluReorderBufferIndex);

  //draw the branch unit
  bool branchUnitSuccessful = model.getBranchUnitSuccessful();
  int branchUnitReorderBufferIndex = model.getBranchUnitReorderBufferIndex();
  view.drawBranchUnit(branchUnitSuccessful, branchUnitReorderBufferIndex);

  //draw the reorder buffer
  Instruction reorderBufferInstructions[reorderBufferSize];
  model.getReorderBufferInstructions(reorderBufferInstructions);
  int** reorderBufferFields = new int*[reorderBufferSize];
  for(int i = 0; i < reorderBufferSize; i++) {
    reorderBufferFields[i] = new int[4];
  }
  model.getReorderBufferFields(reorderBufferFields);
  view.drawReorderBuffer(reorderBufferInstructions, reorderBufferFields);

  //draw the register file
  int registerValues[numOfRegisters];
  int scoreBoard[numOfRegisters];
  model.getAllRegisterValues(registerValues);
  model.getScoreBoard(scoreBoard);
  view.drawRegisterFile(registerValues, scoreBoard);

  //draw the memory
  int memoryValues[memorySize];
  model.getAllMemoryValues(memoryValues);
  view.drawMemory(memoryValues);

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