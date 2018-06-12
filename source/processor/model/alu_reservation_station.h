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

  private:
    
    RegisterFile* const registerFile;
    ALU* const alu;

    Instruction nextInstruction;
    int nextReorderBufferIndex;

    const int size;
    Instruction* const instructions;
    int* const reorderBufferIndexes;

    int opcode;
    int* const operands;
    int reorderBufferIndex;

    int dispatchedIndex;

  public:
    ALUReservationStation(RegisterFile* const registerFile, ALU* const alu, const int size);

    void execute();

    bool spaceInBuffer() const;

    void pipe();

    void flush();

    void print() const;

    void getCurrentInstructions(Instruction* const copy) const;

    void getCurrentReorderBufferIndexes(int* const copy) const;

    void setNextInstruction(const Instruction instruction);

    void setNextReorderBufferIndex(const int index);

  private:

    void addInstruction(const Instruction instruction, const int rbi);

    int findFreePosition() const;

    int readyToDispatch(const Instruction instruction) const;

    //dispatch bound fetch
    void dispatch(const Instruction instruction);
};

#endif