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
#include "store_queue.h"
#include "load_queue.h"

//===========================
//class declaration
class Model {

  //private member variables
  private:

    //Instructions to execute
    const Instructions instructions;

    //general stats
    int noOfInstructionsExecuted;
    int noOfClockCycles;
    float noOfInstructionsPerCycle;

    //processor configuration
    const int numArchitecturalRegisters;
    const int numPhysicalRegisters;
    const int memorySize;
    const int issueWindowSize;
    const int aluReservationStationSize;
    const int branchUnitReservationStationSize;
    const int storeQueueSize;
    const int loadQueueSize;
    const int reorderBufferSize;
    const int numALUs;

    //registers
    int pc;

    //status flags
    bool branchPrediction;
    bool runningFlag;

    //components
    RegisterFile registerFile;
    Memory memory;
    ReorderBuffer reorderBuffer;
    FetchUnit fetchUnit;
    DecodeIssueUnit decodeIssueUnit;
    ALU* const alu;
    ALUReservationStation aluReservationStation;
    BranchUnit branchUnit;
    BranchUnitReservationStation branchUnitReservationStation;
    LoadStoreUnit loadStoreUnit;
    StoreQueue storeQueue;
    LoadQueue loadQueue;

  //public functions
  public:

    Model(const Instructions instructions);

    void cycle();

    void printInfo() const;

  private:

    void run();

    void fetch();

    void decodeIssue();

    void dispatch();

    void execute();

    void commit();

    void pipe();

    void flushPipeline();

    void updateStats();

  //getters and setters
  public:

    int getNumRegisters() const;

    int getMemorySize() const;

    int getIssueWindowSize() const;

    bool getRunningFlag() const;

    int getNoOfInstructionsExecuted() const;

    int getNoOfClockCycles() const;

    float getNoOfInstructionsExecutedPerCycle() const;

    int getPC() const;

    void getRenameTable(int copy[]) const;

    void getArchitecturalRegisterValues(int copy[]) const;

    void getLatestArchitecturalRegisterValues(int copy[]) const;

    void getAllMemoryValues(int copy[]) const;

    void getFetchUnitInstructions(Instruction copy[]) const;

    void getDecodeIssueUnitInstructions(Instruction copy[]) const;

    void getDecodeIssueUnitReorderBufferIndexes(int copy[]) const;

    int getAluReservationStationSize() const;

    void getAluReservationStationInstructions(Instruction copy[]) const;

    void getAluReservationStationReorderBufferIndexes(int copy[]) const;

    void getAluReservationStationOperandTypes(OperandType copy[][3]) const;

    int getBranchUnitReservationStationSize() const;

    void getBranchUnitReservationStationInstructions(Instruction copy[]) const;

    void getBranchUnitReservationStationReorderBufferIndexes(int copy[]) const;

    void getBranchUnitReservationStationOperandTypes(OperandType copy[][3]) const;

    int getStoreQueueSize() const;

    void getStoreQueueInstructions(Instruction copy[]) const;

    void getStoreQueueReorderBufferIndexes(int copy[]) const;

    void getStoreQueueValidBits(bool copy[][3]) const;

    int getLoadQueueSize() const;

    void getLoadQueueInstructions(Instruction copy[]) const;

    void getLoadQueueReorderBufferIndexes(int copy[]) const;

    void getLoadQueueValidBits(bool copy[][3]) const;

    int getNumALUs() const;

    void getAluResults(int copy[]) const;

    int getReorderBufferSize() const;

    void getAluReorderBufferIndexes(int copy[]) const;

    bool getBranchUnitSuccessful() const;

    int getBranchUnitReorderBufferIndex() const;

    void getReorderBufferInstructions(Instruction copy[]) const;

    void getReorderBufferFields(int copy[][ReorderBufferIndex::COUNT]) const;

    int getReorderBufferTailIndex() const;

    int getReorderBufferHeadIndex() const;
};

#endif