//===========================
//include guard
#ifndef PROCESSOR_H
#define PROCESSOR_H

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
    Processor(Instructions instructions);

    void start();

    void fetch();

    void decodeIssue();

    void dispatch();

    void execute();

    void commit();

    void flushPipeline();

    void printInfo();
};

#endif