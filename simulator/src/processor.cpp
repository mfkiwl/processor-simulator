#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "instructions.h"
#include "register_file.h"
#include "memory.h"
#include "fetch_unit.h"
#include "decode_unit.h"
#include "alu.h"
#include "branch_unit.h"
#include "memory_unit.h"


class Processor {

    //Instruction info
    int noOfInstructions;
    int noOfInstructionsExecuted;
	Instruction *instructions;

    //components
    RegisterFile registerFile;
    Memory memory;
    FetchUnit fetchUnit;
    DecodeUnit decodeUnit;
    ALU alu;
    BranchUnit branchUnit;
    MemoryUnit memoryUnit;

    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor(int noOfInstructions, Instruction *instructions) : 
            noOfInstructions(noOfInstructions),
            noOfInstructionsExecuted(0), 
            instructions(instructions),
            registerFile(8), 
            memory(16),
            fetchUnit(instructions, registerFile),
            decodeUnit(fetchUnit),
            alu(registerFile, decodeUnit), 
            branchUnit(registerFile, decodeUnit),
            memoryUnit(memory, registerFile, decodeUnit)
            {}


        void start() {
        	printInfo();
        	printf("Keep pressing ENTER to step through the program\n");

            //step through the program
        	while(registerFile.getpc() < noOfInstructions) {

                //hold up the program
                char str[3];
                fgets(str, 2, stdin);

                //fetch, decode, execute
        	    Instruction i = Fetch();
                DecodeExecute(i);

                //If we have branched then dont increment the program counter
                if(i.opcode != 3) {
                  registerFile.incpc();
                }

                //update info
                noOfInstructionsExecuted++;
                printInfo();
            }
        }


        Instruction Fetch() {
            Instruction i = instructions[registerFile.getpc()];
            return i;
        }

        void DecodeExecute(Instruction i) {
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
        		branchUnit.B(i.operands[0]);
        		break;

                case 4:
                memoryUnit.LD(i.operands[0], i.operands[1], i.operands[2]);
                break;

                case 5:
                memoryUnit.STR(i.operands[0], i.operands[1], i.operands[2]);
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

int main(int argc, char *argv[]) {
    //If command line arguments are incorrect then stop program
    if(argc != 2) {
        printf("Machine code file not valid.\n");
        return 1;
    }

    //get the instructions from the machine code file
    std::string inputFileName(argv[1]);
    int numOfInstructions = getNumOfInstructions(inputFileName);
    Instruction *instructions = NULL;
    if(numOfInstructions != -1) {
        instructions = getInstructions(inputFileName, numOfInstructions);
    }

    //create processor object and start processing
    if(numOfInstructions != -1 && instructions != NULL) {
	    Processor processor(numOfInstructions,instructions);
	    processor.start();
    }
	return 0;
}
