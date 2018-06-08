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

  private:

    //processor configuration
    const int numOfRegisters;
    const int memorySize;
    const int aluReservationStationSize;
    const int branchUnitReservationStationSize;
    const int loadStoreUnitReservationStationSize;

    //general stats
    int noOfInstructionsExecuted;
    int noOfClockCycles;
    float noOfInstructionsPerCycle;

    //Instructions to execute
    const Instructions instructions;

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
    Model(const Instructions instructions, const int numOfRegisters, const int memorySize, 
  const int aluReservationStationSize, const int branchUnitReservationStationSize, 
  const int loadStoreUnitReservationStationSize);

    void updateStats();

    void cycle();

    void run();

    void fetch();

    void decodeIssue();

    void dispatch();

    void execute();

    void commit();

    void flushPipeline();

    void printInfo() const;

    //===============================
    // getter functions

    int getRunningFlag() const;

    int getNumOfRegisters() const;

    int getAluReservationStationSize() const;

    int getBranchUnitReservationStationSize() const;

    int getLoadStoreUnitReservationStationSize() const;

    int getMemorySize() const;

    int getNoOfInstructionsExecuted() const;

    int getNoOfClockCycles() const;

    float getNoOfInstructionsExecutedPerCycle() const;

    int getPC() const;

    void getAllRegisterValues(int* const copy) const;

    void getScoreBoard(int* const copy) const;

    void getAllMemoryValues(int* const copy) const;

    Instruction getFetchUnitInstruction() const;

    Instruction getDecodeIssueUnitInstruction() const;

    void getAluReservationStationInstructions(Instruction* const copy) const;

    void getBranchUnitReservationStationInstructions(Instruction* const copy) const;

    void getLoadStoreUnitReservationStationInstructions(Instruction* const copy) const;
};

#endif