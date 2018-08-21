//===========================
//include guard
#ifndef ALU_H
#define ALU_H

//===========================
//forward declared dependencies
class ReorderBuffer;
class ALUReservationStation;
class LoadStoreUnitReservationStation;
class BranchUnitReservationStation;

//===========================
//class declaration
class ALU {

  //private member variables
  private:

    //forward components
    ReorderBuffer* reorderBuffer;
    ALUReservationStation* aluReservationStation;
    LoadStoreUnitReservationStation* loadStoreUnitReservationStation;
    BranchUnitReservationStation* branchUnitReservationStation;

    //next information
    int nextOpcode;
    int nextOperands[3];
    int nextReorderBufferIndex;

    //current information
    int opcode;
    int operands[3];
    int reorderBufferIndex;

    //instruction result
    int result;

  //public functions
  public:

    ALU();

    ALU(ReorderBuffer* const reorderBuffer, ALUReservationStation* const aluReservationStation);

    void execute();

    void pipe();

    void flush();

  //getters and setters
  public:

    void setNextOpcode(const int x);

    void setNextOperands(const int x[3]);

    void setNextReorderBufferIndex(const int i);

    int getResult() const;

    int getReorderBufferIndex() const;

    void setReorderBufferPointer(ReorderBuffer* p);

    void setALUReservationStationPointer(ALUReservationStation* p);

    void setLoadStoreUnitReservationStationPointer(LoadStoreUnitReservationStation* p);

    void setBranchUnitReservationStationPointer(BranchUnitReservationStation* p);
};

#endif