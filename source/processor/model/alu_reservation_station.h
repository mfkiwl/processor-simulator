//===========================
//include guard
#ifndef ALU_RESERVATION_STATION_H
#define ALU_RESERVATION_STATION_H

//===========================
//forward declared dependencies
class RegisterFile;
class ALU;
class Instruction;

//===========================
//class declaration
class ALUReservationStation {

  private:
    
    RegisterFile* const registerFile;
    ALU* const alu;

    const int size;
    Instruction* instructions;
    int* reorderBufferIndexes;

    int opcode;
    int* operands;
    int reorderBufferIndex;

  public:
    ALUReservationStation(RegisterFile* const registerFile, ALU* const alu, const int size);

    void execute();

    void addInstruction(const Instruction instruction, const int rbi);

    int findFreePosition();

    void pipe();

    void flush();

    void print() const;

    void getCurrentInstructions(Instruction* const copy) const;

  private:

    int readyToDispatch(const Instruction instruction) const;

    //dispatch bound fetch
    void dispatch(const Instruction instruction);
};

#endif