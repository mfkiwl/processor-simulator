#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "instruction.h"
#include "register_file.h"
#include "memory.h"
#include "alu.h"
#include "branch_unit.h"


class Processor {
	std::vector<Instruction> instructions;
    int noOfInstructionsExecuted;
    //components
    RegisterFile registerFile;
    Memory memory;
    ALU alu;
    BranchUnit bu;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor(std::vector<Instruction> a) : instructions(a), noOfInstructionsExecuted(0), registerFile(8), memory(16, registerFile), alu(registerFile), bu(registerFile) {}

        void start() {
        	printInfo();
        	printf("Keep pressing ENTER to step through the program\n");
        	while(registerFile.getpc() < instructions.size()) {
                //step through the program
                char str[3];
                fgets(str, 2, stdin);
        	    Instruction i = instructions[registerFile.getpc()];
                execute(i);
                if(i.assembly[0] != 'B') {
                  registerFile.incpc();
                }
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

                case 4:
                memory.LD(i.operands[0], i.operands[1], i.operands[2]);
                break;

                case 5:
                memory.STR(i.operands[0], i.operands[1], i.operands[2]);
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
	instructions.push_back(Instruction("LD R2 R1 0", 4, 2, 1, 0));
    instructions.push_back(Instruction("ADDI R2 R2 1", 1, 2, 2, 1));
    instructions.push_back(Instruction("STR R2 R1 0", 5, 2, 1, 0));
    instructions.push_back(Instruction("ADDI R2 R1 0", 1, 2, 1, 0));
    instructions.push_back(Instruction("LD R3 R1 R0", 4, 3, 1, 0));
    instructions.push_back(Instruction("ADDI R3 R3 1", 1, 3, 3, 1));
    instructions.push_back(Instruction("STR R3 R1 0", 5, 3, 1, 0));
    instructions.push_back(Instruction("ADDI R3 R1 0", 1, 3, 1, 0));
    instructions.push_back(Instruction("LD R4 R1 0", 4, 4, 1, 0));
    instructions.push_back(Instruction("ADDI R4 R4 1", 1, 4, 4, 1));
    instructions.push_back(Instruction("STR R4 R1 0", 5, 4, 1, 0));
    instructions.push_back(Instruction("ADDI R4 R1 0", 1, 4, 1, 0));
    instructions.push_back(Instruction("LD R5 R1 0", 4, 5, 1, 0));
    instructions.push_back(Instruction("ADDI R5 R5 1", 1, 5, 5, 1));
    instructions.push_back(Instruction("STR R5 R1 0", 5, 5, 1, 0));
    instructions.push_back(Instruction("ADDI R5 R1 0", 1, 5, 1, 0));
    instructions.push_back(Instruction("LD R6 R1 0", 4, 6, 1, 0));
    instructions.push_back(Instruction("ADDI R6 R6 1", 1, 6, 6, 1));
    instructions.push_back(Instruction("STR R6 R1 0", 5, 6, 1, 0));
    instructions.push_back(Instruction("ADDI R6 R1 0", 1, 6, 1, 0));
    instructions.push_back(Instruction("LD R7 R1 0", 4, 7, 1, 0));
    instructions.push_back(Instruction("ADDI R7 R7 1", 1, 7, 7, 1));
    instructions.push_back(Instruction("STR R7 R1 0", 5, 7, 1, 0));
    instructions.push_back(Instruction("ADDI R7 R1 0", 1, 7, 1, 0));
    instructions.push_back(Instruction("B 0", 3, 0, 0, 0));
	Processor processor(instructions);
	processor.start();
	return 0;
}
