#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "instruction.h"
#include "register_file.h"
#include "memory.h"
#include "alu.h"
#include "branch_unit.h"


class Processor {

    //Instruction info
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

int getNumOfInstructions(std::string inputFileName) {
    std::ifstream inputFile(inputFileName.c_str());
    if(inputFile.is_open()) {
        int i = 0;
        std::string line;
        while(getline(inputFile,line)) {
            i++;
        }
        return i;
    }
    else {
        printf("Failed to read file.\n");
        return -1;
    }
}

Instruction *getInstructions(std::string inputFileName, int numOfInstructions) {
    //open file
    std::ifstream inputFile(inputFileName.c_str());
    //if file is open then get the instruction information from each line
    if(inputFile.is_open()) {

        //allocating memory to array to store the instructions
        Instruction *instructions = (Instruction*) malloc(sizeof(Instruction) * numOfInstructions);

        //interating through each instruction
        std::string line;
        for(int i = 0; i < numOfInstructions; i++) {
            getline(inputFile,line);

            //get the opcode
            int pos = line.find(" ");
            std::string opcodestr = line.substr(0,pos);
            line = line.substr(pos + 1, line.size());
            int opcode = atoi(opcodestr.c_str());
            instructions[i].opcode = opcode;
            std::cout << opcodestr + " ";
            
            //get the operands
            pos = line.find(" ");
            int j = 0;
            while(pos != std::string::npos) {
                //get operand
                std::string operandstr = line.substr(0, pos);
                int operand = atoi(operandstr.c_str());
                instructions[i].operands[j] = operand;

                //print operand
                std::cout << operand;
                std::cout << " ";

                //update values
                line = line.substr(pos + 1, line.size());
                pos = line.find(" ");
                j++;
            }
            //get the final operand
            std::string operandstr = line.substr(0, line.size());
            int operand = atoi(operandstr.c_str());
            instructions[i].operands[j] = operand;

            //print final operand
            std::cout << operand;
            std::cout << "\n";
        }
        return instructions;
    }
    else {
        printf("Failed to read file.\n");
        return NULL;
    }
}

int main(int argc, char *argv[]) {
    //If command line arguments are incorrect then stop program
    if(argc != 2) {
        printf("Machine code file not valid.\n");
        return 1;
    }
    //print machine code file name
    printf("%s\n", argv[1]);
    std::string inputFileName(argv[1]);

    //extract the instructions from the machine code file
    int numOfInstructions = getNumOfInstructions(inputFileName);
    Instruction *instructions = getInstructions(inputFileName, numOfInstructions);

    //create processor object and start processing
	Processor processor(numOfInstructions,instructions);
	processor.start();
	return 0;
}
