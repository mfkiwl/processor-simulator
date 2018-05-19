//===========================================
// included dependencies
#include <iostream>

#include "processor.h"

//===========================================
// main function
int main(int argc, char *argv[]) {
  std::cout << "\n";

  //If command line arguments are incorrect then stop program
  if(argc != 2) {
    std::cout << "Machine code file not valid.\n";
    return 1;
  }

  //get the instructions from the machine code file
  std::string inputFileName(argv[1]);
  Instructions instructions(inputFileName);

  //create processor object and start processing
  if(instructions.getNumOfInstructions() != -1 && instructions.getInstructions() != NULL) {
    Processor processor(instructions);
    processor.start();
  }
  std::cout << "\n";
  return 0;
}