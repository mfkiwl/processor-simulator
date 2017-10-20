#include <stdio.h>
#include "register_file.h"

class Processor {
    int pc;
    int noOfInstructionsExecuted;
    RegisterFile registerFile;
    public: 
        Processor() {
	        pc = 0;
	        noOfInstructionsExecuted = 0;
        }
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
	printf("Number of registers: %d\n", processor.getNumRegisters());
	printf("This is my processor.\nThanks!\n");
	return 0;
}
