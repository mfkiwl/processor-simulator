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
  model(instructions)
{
  //give the view component necessary information about the processor
  view.setNumOfRegisters(model.getNumOfRegisters());
  view.setMemorySize(model.getMemorySize());
  view.setAluReservationStationSize(model.getAluReservationStationSize());
  view.setBranchUnitReservationStationSize(model.getBranchUnitReservationStationSize());
}

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
  
  //draw the instruction in the fetch unit
  Instruction fetchUnitInstruction = model.getFetchUnitInstruction();
  view.drawFetchUnit(fetchUnitInstruction);

  //draw the instruction in the decode issue unit
  Instruction decodeIssueUnitInstruction = model.getDecodeIssueUnitInstruction();
  view.drawDecodeIssueUnit(decodeIssueUnitInstruction);

  //draw the instructions in the alu reservation station
  Instruction aluReservationStationInstructions[model.getAluReservationStationSize()];
  model.getAluReservationStationInstructions(aluReservationStationInstructions);
  view.drawAluReservationStation(aluReservationStationInstructions);

  //draw the instructions in the branch unit reservation station
  Instruction branchUnitReservationStationInstructions[model.getBranchUnitReservationStationSize()];
  model.getBranchUnitReservationStationInstructions(branchUnitReservationStationInstructions);
  view.drawBranchUnitReservationStation(branchUnitReservationStationInstructions);

  //draw the register file
  int registerValues[model.getNumOfRegisters()];
  model.getAllRegisterValues(registerValues);
  view.drawRegisterFile(registerValues);

  //draw the memory
  int memoryValues[model.getMemorySize()];
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
  model.printInfo();
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

  //Free resources and close SDL
  view.close();

  printf("PROGRAM FINISHED\n");

  return 0;
}