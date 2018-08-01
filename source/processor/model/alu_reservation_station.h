//===========================
//include guard
#ifndef ALU_RESERVATION_STATION_H
#define ALU_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class ALU;

//=================================
// included dependencies
#include "instructions.h"

//===========================
//class declaration
class ALUReservationStation {

  //private member variables
  private:
    
    RegisterFile* const registerFile;
    ALU* const alu;

    const int size;
    Instruction* const instructions;
    int* const reorderBufferIndexes;

    int numReservedSpaces;

    Instruction* const nextInstructions;
    int* const nextReorderBufferIndexes;

    int opcode;
    int* const operands;
    int reorderBufferIndex;

    int dispatchIndex;

  //public functions
  public:

    ALUReservationStation(RegisterFile* const registerFile, ALU* const alu, const int size);

    void execute();

    void pipe();

    void flush();

    void print() const;

    bool freeSpace() const;

    void reserveSpace();

  //private functions
  private:

    int findFreePosition() const;

    void addNextInstructions();

    bool readyToDispatch(const int index) const;

    //dispatch bound fetch
    void fetchOperands(const int index);

  //getters and setters
  public:

    int getSize() const;

    void getCurrentInstructions(Instruction* const copy) const;

    void getCurrentReorderBufferIndexes(int* const copy) const;

    void setNextInstruction(const Instruction instruction, const int rbi);
};

#endif