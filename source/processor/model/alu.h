//===========================
//include guard
#ifndef ALU_H
#define ALU_H

//===========================
//forward declared dependencies
class ReorderBuffer;
class ALUReservationStation;
class StoreQueue;
class LoadQueue;
class BranchUnitReservationStation;

//===========================
//class declaration
class ALU {

  //private member variables
  private:

    //forward components
    ReorderBuffer* reorderBuffer;
    ALUReservationStation* aluReservationStation;
    StoreQueue* storeQueue;
    LoadQueue* loadQueue;
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

    void setStoreQueuePointer(StoreQueue* p);

    void setLoadQueuePointer(LoadQueue* p);

    void setBranchUnitReservationStationPointer(BranchUnitReservationStation* p);
};

#endif