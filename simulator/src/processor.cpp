#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "instructions.h"
#include "register_file.h"
#include "memory.h"
#include "alu.h"
#include "branch_unit.h"
#include "memory_unit.h"
#include "decode_unit.h"
#include "fetch_unit.h"


class Processor {

    //Instruction info
    int noOfInstructions;
    int noOfInstructionsExecuted;
	Instruction *instructions;

    //registers
    int pc;

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
            //instruction info
            noOfInstructions(noOfInstructions),
            noOfInstructionsExecuted(0), 
            instructions(instructions),

            //registers
            pc(0),

            //components
            registerFile(8), 
            memory(16),
            fetchUnit(instructions, &pc, &decodeUnit),
            decodeUnit(&alu, &branchUnit, &memoryUnit),
            alu(&registerFile),
            branchUnit(&pc),
            memoryUnit(&memory, &registerFile)
        {}


        void start() {
        	printInfo();
        	printf("Keep pressing ENTER to step through the program\n");

            //step through the program
        	while(pc < noOfInstructions) {

                //hold up the program
                char str[3];
                fgets(str, 2, stdin);

                //fetch, decode, execute
        	    fetchUnit.run();
                decodeUnit.run();
                alu.run();
                branchUnit.run();
                memoryUnit.run();

                //update info
                noOfInstructionsExecuted++;
                printInfo();
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
