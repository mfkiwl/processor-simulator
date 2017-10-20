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
        	printf("0: %d | 1: %d | 2: %d\n", registerFile.getRegisterValue(0), registerFile.getRegisterValue(1), registerFile.getRegisterValue(2));
            registerFile.setRegisterValue(1, 2);
            registerFile.setRegisterValue(2, 3);
            printf("0: %d | 1: %d | 2: %d\n", registerFile.getRegisterValue(0), registerFile.getRegisterValue(1), registerFile.getRegisterValue(2));
            alu.add(0,1,2);
            printf("0: %d | 1: %d | 2: %d\n", registerFile.getRegisterValue(0), registerFile.getRegisterValue(1), registerFile.getRegisterValue(2));
        }
};


int main(void) {
	Processor processor;
	processor.info();
	return 0;
}
