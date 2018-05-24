#include "controller.h"
#include <stdio.h>

//===========================================
// main function
int main(int argc, char *argv[]) {

  //Getting the Instructions from the file in the command line argument
  if(argc != 2) {
    printf("Machine code file not valid.\n");
    return 1;
  }
  std::string inputFileName(argv[1]);
  Instructions instructions(inputFileName);

  Controller controller(instructions);

  //main functions
  controller.processorMain(instructions);
  controller.viewmain(argc, argv);
  return 0;
}