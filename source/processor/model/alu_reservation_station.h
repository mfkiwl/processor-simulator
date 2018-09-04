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
#include "constants.h"

//===========================
//class declaration
class ALUReservationStation {

  //private member variables
  private:
    
    RegisterFile* const registerFile;
    const int numALUs;
    ALU* const alu;

    const int size;

    Instruction* const nextInstructions;
    OperandType** const nextOperandTypes;
    int* const nextReorderBufferIndexes;

    Instruction* const instructions;
    OperandType** const operandTypes;
    int* const reorderBufferIndexes;

    int numReservedSpaces;

    int* const dispatchIndexes;

  //public functions
  public:

    ALUReservationStation(RegisterFile* const registerFile, const int numALUs, ALU* const alu, const int size);

    void execute();

    void pipe();

    void flush();

    void print() const;

    bool freeSpace() const;

    void reserveSpace();

    void broadcast(int physicalRegister, int value);

  //private functions
  private:

    void checkOperandAvailability();

    int findFreePosition() const;

    void addNextInstructions();

    bool readyToDispatch(const int index) const;

  //getters and setters
  public:

    void setNextInstruction(const Instruction instruction, const OperandType types[], const int rbi);

    void getCurrentInstructions(Instruction copy[]) const;

    void getCurrentReorderBufferIndexes(int copy[]) const;

    void getOperandTypes(OperandType copy[][numOfOperands]) const;
};

#endif