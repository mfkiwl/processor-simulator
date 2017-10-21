#include <stdio.h>
#include "register_file.h"
#include "alu.h"
#include <iostream>

class Instruction {
	public:
        std::string assembly;
        int opcode;
        int operands[3];
    	Instruction(std::string a, int b, int c, int d, int e) : assembly(a), opcode(b) {
    		operands[0] = c;
    		operands[1] = d;
    		operands[2] = e;
    	}
};


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
	Instruction i1("ADDI R1 R1 R2", 0, 1, 1, 2);
	Instruction i2("ADDI R2 R2 R3", 0, 2, 2, 3);
	Instruction i3("ADD R0 R1 R2", 1, 0, 1, 2);
	Instruction i4("SUB R3 R1 R0", 2, 3, 1, 0);
	Instruction instructions[] = {i1, i2, i3, i4};
	Processor processor;
	processor.info();
	return 0;
}
