//===========================================
//included header file containing interface
#include "controller.h"

//===========================================
// included dependencies
#include <stdio.h>

#include "model/processor.h"
#include "view/view.h"

//===========================================
// class implementation

Controller::Controller(Instructions instructions) :
  processor(instructions)
{}

//the original main function for the processor model
int Controller::processorMain(Instructions instructions) {
  
  //running the processor
  char str[3];
  printf("Keep pressing ENTER to step through the program\n");

  processor.printInfo();

  //step through the program
  while(processor.getRunningFlag()) {

    //hold up the program at each clock cycle
    if(str[0] != 'e') {
      fgets(str, 2, stdin);
    }
    
    //perform one clock cycle
    processor.cycle();
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


int Controller::start(Instructions instructions) {

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

  //buffer to hold the user input
  char str[3];

  printf("Keep pressing ENTER to step through the program\n");

  //display initial information
  processor.printInfo();
  view.frame();

  //While application is running
  while( !view.quit && processor.getRunningFlag())
  {

    //hold up the program at each clock cycle
    if(str[0] != 'e') {
      fgets(str, 2, stdin);
    }
    
    //perform one clock cycle
    processor.cycle();
    
    //draw the frame
    view.frame();
  }

  //Free resources and close SDL
  view.close();

  printf("PROGRAM FINISHED\n");

  return 0;
}
