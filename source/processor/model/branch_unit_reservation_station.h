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

  private:
    
    RegisterFile* const registerFile;
    BranchUnit* const branchUnit;

    const int size;
    Instruction* instructions;
    int* reorderBufferIndexes;

    int opcode;
    int* operands;
    int reorderBufferIndex;

  public:
    BranchUnitReservationStation(RegisterFile* const registerFile, BranchUnit* const branchUnit);

    void execute();

    void addInstruction(const Instruction instruction, const int rbi);

    int findFreePosition();

    void pipe();

    void flush();

    void print() const;

  private:

    int readyToDispatch(const Instruction instruction) const;

    //dispatch bound fetch
    void dispatch(const Instruction instruction);

};

#endif