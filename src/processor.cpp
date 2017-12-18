#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <string>

#include "constants.h"
#include "instructions.h"
#include "register_file.h"
#include "reorder_buffer.h"
#include "memory.h"
#include "alu.h"
#include "branch_unit.h"
#include "store_queue.h"
#include "load_queue.h"
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
            reorderBuffer(100, &noOfInstructionsExecuted),
            fetchUnit(instructions, noOfInstructions, &pc, &decodeUnit),
            decodeUnit(&registerFile, &reorderBuffer, &alu, &branchUnit, &loadStoreUnit, &decodeUnitBlockingFlag),
            alu(&registerFile, &reorderBuffer),
            branchUnit(&reorderBuffer, &pc, &flushFlag, &runningFlag),
            loadStoreUnit(&memory, &registerFile, &reorderBuffer, &loadStoreUnitBlockingFlag)
        {}


        void start() {

            cout << "Keep pressing ENTER to step through the program\n";

            printInfo();

            //step through the program
        	while(runningFlag) {

                //hold up the program at each clock cycle
                char str[3];
                //fgets(str, 2, stdin);

                //writeback the result
                writeback();
                reorderBuffer.retire();
                //execute the instruction
                execute();

                //check if we should flush the pipeline
                if(flushFlag == 1) {
                    flushPipeline();
                }

                //decode the instruction
                decode();
                //if the pipeline is not being blocked
                if(!decodeUnitBlockingFlag && !loadStoreUnitBlockingFlag) {
                    //fetch the next instruction
                    fetch();
                    //propogate values through pipeline
                    pipe();
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
            registerFile.resetScoreBoard();
        }

        void printInfo() {
            cout << endl;
            cout << "Number of clock cycles: " << noOfClockCycles << endl;
            cout << "Number of instructions executed: " << noOfInstructionsExecuted << endl;
            float instructionsPerCycle;
            if(noOfClockCycles == 0) {
                instructionsPerCycle = 0;
            }
            else {
                instructionsPerCycle = (float) noOfInstructionsExecuted / (float) noOfClockCycles;
            }
            cout << "Instruction per cycle: " << instructionsPerCycle << endl;
            cout << endl;
            cout << "PC: " << pc << endl;
            registerFile.printRegisters();
            cout << endl;
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
        Processor processor(16, 24, numOfInstructions,instructions);
        processor.start();
    }
    cout << "\n";
    return 0;
}
