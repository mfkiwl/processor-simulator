#include <stdio.h>
#include "register_file.h"

class Processor {
    int pc;
    int noOfInstructionsExecuted;
    RegisterFile registerFile;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor() : registerFile(3), pc(0), noOfInstructionsExecuted(0) {}

        int getpc() {
	        return pc;
        }

        int getNoOfInstructionsExecuted() {
        	return noOfInstructionsExecuted;
        }

        int getNumRegisters() {
        	return registerFile.getNumRegisters();
        }
};


int main(void) {
	Processor processor;
	printf("pc: %d\n", processor.getpc());
	printf("number of instrutions executed: %d\n", processor.getNoOfInstructionsExecuted());
	printf("Number of registers: %d\n", processor.getNumRegisters());
	printf("This is my processor.\nThanks!\n");
	return 0;
}
