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

int Controller::processorMain(Instructions instructions) {
  
  //create processor object and start processing
  if(instructions.getNumOfInstructions() != -1 && instructions.getInstructions() != NULL) {
    
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
  }
  printf("\n");
  return 0;
}

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