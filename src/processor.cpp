#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "constants.h"
#include "instructions.h"
#include "register_file.h"
#include "memory.h"
#include "reorder_buffer.h"
#include "alu.h"
#include "branch_unit.h"
#include "write_buffer.h"
#include "read_buffer.h"
#include "load_store_unit.h"
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
    int decodeUnitBlockingFlag;
    int loadStoreUnitBlockingFlag;

    //components
    RegisterFile registerFile;
    Memory memory;
    ReorderBuffer reorderBuffer;
    FetchUnit fetchUnit;
    DecodeUnit decodeUnit;
    ALU alu;
    BranchUnit branchUnit;
    LoadStoreUnit loadStoreUnit;

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
            decodeUnitBlockingFlag(0),
            loadStoreUnitBlockingFlag(0),

            //components
            registerFile(noOfRegisters), 
            memory(memorySize),
            reorderBuffer(100),
            fetchUnit(instructions, noOfInstructions, &pc, &decodeUnit),
            decodeUnit(&registerFile, &alu, &branchUnit, &loadStoreUnit, &decodeUnitBlockingFlag),
            alu(&registerFile, &noOfInstructionsExecuted),
            branchUnit(&pc, &flushFlag, &runningFlag, &noOfInstructionsExecuted),
            loadStoreUnit(&memory, &registerFile, &noOfInstructionsExecuted, &loadStoreUnitBlockingFlag)
        {}


        void start() {

            cout << "Keep pressing ENTER to step through the program\n";

            printInfo();

            //step through the program
        	while(runningFlag) {

                //hold up the program at each clock cycle
                char str[3];
                fgets(str, 2, stdin);

                //if the pipeline is not being blocked
                if(!decodeUnitBlockingFlag && !loadStoreUnitBlockingFlag) {
                    //propogate values through pipeline
                    pipe();
                    //fetch the next instruction
                    fetch();
                }
                //decode the instruction
                decode();
                //execute the instruction
                execute();
                //writeback the result
                writeback();
                
                //check if we received a message to flush the pipeline before fetching the next instruction
                if(flushFlag == 1) {
                    //if so then flush the pipeline
                    flushPipeline();
                }

                //update info
                noOfClockCycles++;

                //print register info
                printInfo();
            }

            cout << "PROGRAM FINISHED\n";
        }

        void pipe() {
            fetchUnit.pipe();
            decodeUnit.pipe();
        }

        void fetch() {
            fetchUnit.execute();
        }

        void decode() {
            decodeUnit.execute();
        }

        void execute() {
            alu.execute();
            branchUnit.execute();
            loadStoreUnit.execute();
        }

        void writeback() {
            alu.writeback();
            loadStoreUnit.writeback();
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
