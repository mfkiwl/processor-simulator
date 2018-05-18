#include "processor.h"


int main(int argc, char *argv[]) {
  cout << "\n";

  //If command line arguments are incorrect then stop program
  if(argc != 2) {
    cout << "Machine code file not valid.\n";
    return 1;
  }

  //get the instructions from the machine code file
  string inputFileName(argv[1]);
  Instructions instructions(inputFileName);

  //create processor object and start processing
  if(instructions.getNumOfInstructions() != -1 && instructions.getInstructions() != NULL) {
    Processor processor(instructions);
    processor.start();
  }
  cout << "\n";
  return 0;
}