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
    int noOfInstructions;
	Instruction *instructions;
    int noOfInstructionsExecuted;

    //components
    RegisterFile registerFile;
    Memory memory;
    ALU alu;
    BranchUnit bu;
    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor(int a, Instruction *b) : 
            noOfInstructions(a),
            instructions(b),
            noOfInstructionsExecuted(0), 
            registerFile(8), 
            memory(16, registerFile), 
            alu(registerFile), 
            bu(registerFile) {}

        void start() {
        	printInfo();
        	printf("Keep pressing ENTER to step through the program\n");
        	while(registerFile.getpc() < noOfInstructions) {
                //step through the program
                char str[3];
                fgets(str, 2, stdin);
        	    Instruction i = instructions[registerFile.getpc()];
                execute(i);
                if(i.opcode != 3) {
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
    Instruction instructions[25] = {
        {4,2,1,0},
        {1,2,2,1},
        {5,2,1,0},
        {1,2,1,0},
        {4,3,1,0},
        {1,3,3,1},
        {5,3,1,0},
        {1,3,1,0},
        {4,4,1,0},
        {1,4,4,1},
        {5,4,1,0},
        {1,4,1,0},
        {4,5,1,0},
        {1,5,5,1},
        {5,5,1,0},
        {1,5,1,0},
        {4,6,1,0},
        {1,6,6,1},
        {5,6,1,0},
        {1,6,1,0},
        {4,7,1,0},
        {1,7,7,1},
        {5,7,1,0},
        {1,7,1,0},
        {3,0,0,0}
    };

	Processor processor(25,instructions);
	processor.start();
	return 0;
}
