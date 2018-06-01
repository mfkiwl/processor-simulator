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

  //Render texture to screen
  view.renderImage();

  Instruction currentInstruction = model.getCurrentInstruction();

  //Render current frame
  view.renderText(std::to_string(currentInstruction.opcode));

  //Update screen
  view.showScreen();
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