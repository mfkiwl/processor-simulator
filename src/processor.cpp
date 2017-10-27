#include <stdio.h>
#include <iostream>
#include <vector>

#include "instruction.h"
#include "register_file.h"
#include "alu.h"
#include "branch_unit.h"


class Processor {
	std::vector<Instruction> instructions;
    int noOfInstructionsExecuted;
    RegisterFile registerFile;
    ALU alu;
    BranchUnit bu;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor(std::vector<Instruction> a) : instructions(a), noOfInstructionsExecuted(0), registerFile(8), alu(registerFile), bu(registerFile) {}

        void start() {
        	printInfo();
        	while(registerFile.getpc() < instructions.size()) {
        	    Instruction i = instructions[registerFile.getpc()];
                execute(i);
                registerFile.incpc();
                noOfInstructionsExecuted++;
                printInfo();
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

        		case 3:
        		bu.B(i.operands[0]);
        		break;
        	}
        }

        void printInfo() {
        	printf("\n");
        	printf("Instructions executed: %d\n", noOfInstructionsExecuted);
        	registerFile.printRegisters();
        	printf("\n");
        }
};


int main(void) {
	std::vector<Instruction> instructions;
	instructions.push_back(Instruction("ADDI R1 R1 R2", 1, 1, 1, 2));
	instructions.push_back(Instruction("ADDI R2 R2 R3", 1, 2, 2, 3));
	instructions.push_back(Instruction("ADD R0 R1 R2", 0, 0, 1, 2));
	instructions.push_back(Instruction("SUB R3 R1 R0", 2, 3, 1, 0));
	instructions.push_back(Instruction("ADDI R1 R1 R2", 1, 1, 1, 2));
	instructions.push_back(Instruction("ADDI R1 R1 R2", 1, 1, 1, 2));
	//minstructions.push_back(Instruction("B 0", 3, 0, 0, 0));
	Processor processor(instructions);
	processor.start();
	return 0;
}
