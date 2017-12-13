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

using namespace std;


class Processor {

    //information
    int noOfRegisters;
    int memorySize;

    //general stats
    int noOfInstructionsExecuted;
    int noOfClockCycles;

    //Instruction info
    int noOfInstructions;
    Instruction *instructions;

    //registers
    int pc;

    //status flags
    int flushFlag;
    int runningFlag;
    int blockingFlag;

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
        Processor(int noOfRegisters, int memorySize, int noOfInstructions, Instruction *instructions) : 

            //processor configuration
            noOfRegisters(noOfRegisters),
            memorySize(memorySize),

            //general stats
            noOfInstructionsExecuted(0),
            noOfClockCycles(0),

            //instruction info
            noOfInstructions(noOfInstructions),
            instructions(instructions),

            //registers
            pc(1),

            //status flags
            flushFlag(0),
            runningFlag(1),
            blockingFlag(0),

            //components
            registerFile(noOfRegisters), 
            memory(memorySize),
            fetchUnit(instructions, noOfInstructions, &pc, &decodeUnit),
            decodeUnit(&registerFile, &alu, &branchUnit, &memoryUnit, &blockingFlag),
            alu(&registerFile, &noOfInstructionsExecuted),
            branchUnit(&pc, &flushFlag, &runningFlag, &noOfInstructionsExecuted),
            memoryUnit(&memory, &registerFile, &noOfInstructionsExecuted, &blockingFlag)
        {}


        void start() {

            cout << "Keep pressing ENTER to step through the program\n";

            printInfo();

            //step through the program
        	while(runningFlag) {

                //hold up the program at each clock cycle
                char str[3];
                fgets(str, 2, stdin);

                //run each unit
                alu.execute();
                branchUnit.execute();
                memoryUnit.execute();
                decodeUnit.execute();

                //check if we received a message to flush the pipeline before fetching the next instruction
                if(flushFlag == 1) {
                    //if so then flush the pipeline
                    flushPipeline();
                }

                //only if the pipeline is not being blocked
                if(!blockingFlag) {

                    //propogate values through pipeline
                    fetchUnit.pipe();
                    decodeUnit.pipe();

                    fetchUnit.execute();
                }

                //update info
                noOfClockCycles++;

                //print register info
                printInfo();
            }

            cout << "PROGRAM FINISHED\n";
        }

        void flushPipeline() {
            alu.flush();
            branchUnit.flush();
            decodeUnit.flush();
            fetchUnit.flush();
            decodeUnit.flush();
            flushFlag = 0;
        }

        void printInfo() {
            cout << "\n";
            cout << "Number of clock cycles: " << noOfClockCycles << "\n";
            cout << "Number of instructions executed: " << noOfInstructionsExecuted << "\n";
            float instructionsPerCycle;
            if(noOfClockCycles == 0) {
                instructionsPerCycle = 0;
            }
            else {
                instructionsPerCycle = (float) noOfInstructionsExecuted / (float) noOfClockCycles;
            }
            cout << "Instruction per cycle: " << instructionsPerCycle << "\n";
            cout << "\n";
            cout << "PC: " << pc << "\n";
            registerFile.printRegisters();
            cout << "\n";

        }
};

int main(int argc, char *argv[]) {
    cout << "\n";

    //If command line arguments are incorrect then stop program
    if(argc != 2) {
        cout << "Machine code file not valid.\n";
        return 1;
    }

    //get the instructions from the machine code file
    string inputFileName(argv[1]);
    int numOfInstructions = getNumOfInstructions(inputFileName);
    Instruction *instructions = NULL;
    if(numOfInstructions != -1) {
        instructions = getInstructions(inputFileName, numOfInstructions);
    }

    //create processor object and start processing
    if(numOfInstructions != -1 && instructions != NULL) {
        Processor processor(16, 64, numOfInstructions,instructions);
        processor.start();
    }
    cout << "\n";
    return 0;
}
