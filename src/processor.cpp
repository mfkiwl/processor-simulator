#include <stdio.h>
#include "register_file.h"
#include "alu.h"
#include <iostream>
#include <vector>

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
	std::vector<Instruction> instructions;
    int pc;
    int noOfInstructionsExecuted;
    RegisterFile registerFile;
    ALU alu;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor(std::vector<Instruction> a) : instructions(a), pc(0), noOfInstructionsExecuted(0), registerFile(8), alu(registerFile) {}

        void start() {
        	//printf("PC: %d\n", pc);
        	//printf("Instructions executed: %d\n", noOfInstructionsExecuted);
        	printf("Registers: ");
        	registerFile.printRegisterValues();
        	while(pc < instructions.size()) {
        	    Instruction i = instructions[pc];
                execute(i);
                pc++;
                noOfInstructionsExecuted++;
                //printf("PC: %d\n", pc);
        	    //printf("Instructions executed: %d\n", noOfInstructionsExecuted);
        	    printf("Registers: ");
        	    registerFile.printRegisterValues();
            }
        }

        void execute(Instruction i) {
        	switch(i.opcode) {
        		case 0:
        		alu.ADD(i.operands[0], i.operands[1], i.operands[2]);
        		break;

        		case 1:
        		alu.ADDI(i.operands[0], i.operands[1], i.operands[2]);
        		break;

        		case 2:
        		alu.SUB(i.operands[0], i.operands[1], i.operands[2]);
        		break;
        	}
        }

        void printInstructions() {
        	for(int i = 0; i < instructions.size(); i++) {
        		std::cout << instructions[i].assembly << std::endl;
        	}
        }

        void printpc() {
        	printf("%d\n", pc);
        }
};


int main(void) {
	std::vector<Instruction> instructions;
	instructions.push_back(Instruction("ADDI R1 R1 R2", 1, 1, 1, 2));
	instructions.push_back(Instruction("ADDI R2 R2 R3", 1, 2, 2, 3));
	instructions.push_back(Instruction("ADD R0 R1 R2", 0, 0, 1, 2));
	instructions.push_back(Instruction("SUB R3 R1 R0", 2, 3, 1, 0));
	Processor processor(instructions);
	processor.start();
	return 0;
}
