#include <stdio.h>
#include "register_file.h"
#include "alu.h"

class Processor {
    int pc;
    int noOfInstructionsExecuted;
    RegisterFile registerFile;
    ALU alu;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor() : pc(0), noOfInstructionsExecuted(0), registerFile(8), alu(registerFile) {}

        void info() {
        	registerFile.printRegisterValues();
            alu.ADDI(1,1,2);
            registerFile.printRegisterValues();
            alu.ADDI(2,2,3);
            registerFile.printRegisterValues();
            alu.ADD(0,1,2);
            registerFile.printRegisterValues();
            alu.SUB(3,1,0);
            registerFile.printRegisterValues();
        }
};


int main(void) {
	Processor processor;
	processor.info();
	return 0;
}
