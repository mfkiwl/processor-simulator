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

Controller::Controller(Instructions instructions) :
  model(instructions)
{}

//the original main function for the processor model
int Controller::modelMain(Instructions instructions) {
  
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
int Controller::viewmain(int argc, char *argv[])
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
	while( !view.quit )
	{
        view.frame();
	}

	//Free resources and close SDL
	view.close();

	return 0;
}

//Render the current frame with the appropriate information from the processor
void Controller::updateView() {
  //handle events
  view.eventHandler();

  //Clear screen
  view.clearScreen();

  //draw the register file
  view.drawRegisterFile();

  //render the number of instructions executed
  int noOfInstructionsExecuted = model.getNoOfInstructionsExecuted();
  std::string text = "Number of instructions executed: " + std::to_string(noOfInstructionsExecuted);
  view.renderText(0, 0, text);

  //render the number of clock cycles performed
  int noOfClockCycles = model.getNoOfClockCycles();
  text = "Number of clock cycles performed: " + std::to_string(noOfClockCycles);
  view.renderText(0, 20, text);

  //render the number of the instructions executed per clock cycle
  float noOfInstructionsExecutedPerCycle = model.getNoOfInstructionsExecutedPerCycle();
  text = "Number of instructions executed per cycle: " + std::to_string(noOfInstructionsExecutedPerCycle);
  view.renderText(0, 40, text);

  //Update screen
  view.updateScreen();
}


int Controller::start(Instructions instructions) {

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
  while( !view.quit && model.getRunningFlag())
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