//===========================
//include guard
#ifndef MODEL_H
#define MODEL_H

//=================================
// included dependencies
#include "instructions.h"
#include "register_file.h"
#include "memory.h"
#include "reorder_buffer.h"
#include "fetch_unit.h"
#include "decode_issue_unit.h"
#include "alu.h"
#include "alu_reservation_station.h"
#include "branch_unit.h"
#include "branch_unit_reservation_station.h"
#include "load_store_unit.h"
#include "load_store_unit_reservation_station.h"

//===========================
//class declaration
class Model {

    //information
    int memorySize;

    //general stats
    int noOfInstructionsExecuted;
    int noOfClockCycles;
    float noOfInstructionsPerCycle;

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
    Model(Instructions instructions);

    void cycle();

    void run();

    void fetch();

    void decodeIssue();

    void dispatch();

    void execute();

    void commit();

    void flushPipeline();

    void printInfo();

    int getRunningFlag();

    //===============================
    // getter functions

    int getNoOfInstructionsExecuted();

    int getNoOfClockCycles();

    float getNoOfInstructionsExecutedPerCycle();

    int getPC();

    Instruction getCurrentInstruction();
};

#endif