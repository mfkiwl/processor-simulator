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
    
    RegisterFile* registerFile;
    ALU* alu;

    int size;
    Instruction* instructions;
    int* reorderBufferIndexes;

    int opcode;
    int* operands;
    int reorderBufferIndex;

  public:
    ALUReservationStation(RegisterFile* registerFile, ALU* alu);

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