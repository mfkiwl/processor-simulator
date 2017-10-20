#include <stdio.h>
#include "register_file.h"

class Processor {
    int pc;
    int noOfInstructionsExecuted;
    RegisterFile registerFile;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor() : registerFile(8), pc(0), noOfInstructionsExecuted(0) {}

        void info() {
            printf("pc: %d\n", pc);
	        printf("number of instrutions executed: %d\n", noOfInstructionsExecuted);
            printf("Number of registers: %d\n", registerFile.getNumRegisters());
            int r = 7;
            printf("Value at register %d is %d\n", r, registerFile.getRegisterValue(r));
            printf("This is my processor.\nThanks!\n");
        }
};


int main(void) {
	Processor processor;
	processor.info();
	return 0;
}
