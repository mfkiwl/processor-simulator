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
#include "alu_reservation_station.h"
#include "branch_unit.h"
#include "branch_unit_reservation_station.h"
#include "store_buffer.h"
#include "load_buffer.h"
#include "load_store_unit.h"
#include "load_store_unit_reservation_station.h"
#include "decode_issue_unit.h"
#include "fetch_unit.h"

using namespace std;


class Processor {

    //information
    int memorySize;

    //general stats
    int noOfInstructionsExecuted;
    int noOfClockCycles;

    //Instructions to execute
    Instructions instructions;

    //registers
    int pc;

    //status flags
    int flushFlag;
    int runningFlag;
    int decodeUnitBlockingFlag;

    //components
    RegisterFile registerFile;
    Memory memory;
    ReorderBuffer reorderBuffer;
    FetchUnit fetchUnit;
    DecodeIssueUnit decodeIssueUnit;
    ALU alu;
    ALUReservationStation aluReservationStation;
    BranchUnit branchUnit;
    BranchUnitReservationStation branchUnitReservationStation;
    LoadStoreUnit loadStoreUnit;
    LoadStoreUnitReservationStation loadStoreUnitReservationStation;

    public:
    	//Classes needed to be initialised in the uniform initialiser list
        Processor(Instructions instructions) : 

            //processor configuration
            memorySize(24),

            //general stats
            noOfInstructionsExecuted(0),
            noOfClockCycles(0),

            //instruction info
            instructions(instructions),

            //registers
            pc(1),

            //status flags
            flushFlag(0),
            runningFlag(1),
            decodeUnitBlockingFlag(0),

            //components
            registerFile(), 
            memory(memorySize),
            reorderBuffer(&registerFile, &memory, &pc, &flushFlag, &runningFlag, &noOfInstructionsExecuted),
            fetchUnit(instructions, &pc, &decodeIssueUnit),
            decodeIssueUnit(&registerFile, &reorderBuffer, &aluReservationStation, &branchUnitReservationStation, &loadStoreUnitReservationStation, &decodeUnitBlockingFlag),
            alu(&registerFile, &reorderBuffer),
            aluReservationStation(&registerFile, &alu),
            branchUnit(&reorderBuffer),
            branchUnitReservationStation(&registerFile, &branchUnit),
            loadStoreUnit(&memory, &reorderBuffer),
            loadStoreUnitReservationStation(&registerFile, &reorderBuffer, &loadStoreUnit)
        {}


        void start() {

            cout << "Keep pressing ENTER to step through the program\n";

            printInfo();

            //step through the program
        	while(runningFlag) {

                //hold up the program at each clock cycle
                char str[3];
                fgets(str, 2, stdin);

                //writeback the results
                commit();

                //check if we should flush the pipeline
                if(flushFlag == 1) {
                    flushPipeline();
                }

                //execute the instruction
                execute();

                //dispatch instructions from the reservation stations
                dispatch();

                //decode the instruction
                decodeIssue();

                 //if the pipeline is not being blocked
                if(!decodeUnitBlockingFlag) {
                    //fetch the next instruction
                    fetch();
                    //propogate outputs of the decode/issue unit and the fetch unit through pipeline
                    decodeIssueUnit.pipe();
                    fetchUnit.pipe();
                }
                else {
                    printf("\n");
                    printf("BLOCKING ISSUE\n");
                    printf("\n");
                }

                //propogate the outputs of the reservation stations through the pipeline
                alu.pipe();
                branchUnit.pipe();
                loadStoreUnit.pipe();

                //propogate the outputs of the reservation stations through the pipeline
                aluReservationStation.pipe();
                branchUnitReservationStation.pipe();
                loadStoreUnitReservationStation.pipe();

                //update info
                noOfClockCycles++;

                //print register info
                printInfo();
            }

            cout << "PROGRAM FINISHED\n";
        }

        void fetch() {
            fetchUnit.execute();
        }

        void decodeIssue() {
            decodeIssueUnit.execute();
        }

        void dispatch() {
            aluReservationStation.execute();
            branchUnitReservationStation.execute();
            loadStoreUnitReservationStation.execute();
        }

        void execute() {
            alu.execute();
            loadStoreUnit.execute();
            branchUnit.execute();
        }

        void commit() {
            reorderBuffer.retire();
        }

        void flushPipeline() {
            //flush decode unit
            decodeIssueUnit.flush();
            //flush reservation stations
            aluReservationStation.flush();
            branchUnitReservationStation.flush();
            loadStoreUnitReservationStation.flush();
            //flush execution units
            alu.flush();
            branchUnit.flush();
            loadStoreUnit.flush();
            //flush reorder buffer
            reorderBuffer.flush();
            //reset the register file scoreboard
            registerFile.resetScoreBoard();
            //reset the flush flag
            flushFlag = 0;
            decodeUnitBlockingFlag = 0;

            printf("FLUSHING PIPELINE!\n");
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
            //memory.print();
            //registerFile.printScoreBoard();
            //fetchUnit.print();
            //decodeIssueUnit.print();
            //aluReservationStation.print();
            //branchUnitReservationStation.print();
            //loadStoreUnitReservationStation.print();
            //reorderBuffer.print();
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
    Instructions instructions(inputFileName);

    //create processor object and start processing
    if(instructions.getNumOfInstructions() != -1 && instructions.getInstructions() != NULL) {
        Processor processor(instructions);
        processor.start();
    }
    cout << "\n";
    return 0;
}
