//===========================
//include guard
#ifndef LOAD_STORE_UNIT_RESERVATION_STATION_H
#define LOAD_STORE_UNIT_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class ReorderBuffer;
class LoadStoreUnit;
class Instruction;

//===========================
//class declaration
class LoadStoreUnitReservationStation {

    RegisterFile* const registerFile;
    ReorderBuffer* const reorderBuffer;
    LoadStoreUnit* const loadStoreUnit;

    int tail;
    int head;
    const int size;
    Instruction* const instructions;
    int* const reorderBufferIndexes;

    int opcode;
    int operands[3];
    int reorderBufferIndex;

  public:
    LoadStoreUnitReservationStation(RegisterFile* const registerFile, ReorderBuffer* const reorderBuffer, LoadStoreUnit* const loadStoreUnit);

    void execute();

    void addInstruction(const Instruction instruction, const int rbi);

    int spaceInQueue() const;

    void pipe();

    void flush();

    void print() const;

  private:

    int readyToDispatch(const int index) const;

    //dispatch bound fetch
    void dispatch(const int index);

};

#endif