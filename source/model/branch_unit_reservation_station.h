//===========================
//include guard
#ifndef BRANCH_UNIT_RESERVATION_STATION_H
#define BRANCH_UNIT_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class BranchUnit;
class Instruction;

//===========================
//class declaration
class BranchUnitReservationStation {
    
    RegisterFile* registerFile;
    BranchUnit* branchUnit;

    int size;
    Instruction* instructions;
    int* reorderBufferIndexes;

    int opcode;
    int* operands;
    int reorderBufferIndex;

  public:
    BranchUnitReservationStation(RegisterFile* registerFile, BranchUnit* branchUnit);

    void execute();

    void addInstruction(Instruction instruction, int rbi);

    int findFreePosition();

    void pipe();

    void flush();

    void print();

  private:

    int readyToDispatch(Instruction instruction);

    //dispatch bound fetch
    void dispatch(Instruction instruction);

};

#endif