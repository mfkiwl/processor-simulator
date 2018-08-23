//===========================
//include guard
#ifndef LOAD_BUFFER_H
#define LOAD_BUFFER_H

//===========================
//forward declared dependencies
class Memory;
class ReorderBuffer;
class ALUReservationStation;

//===========================
//class declaration
class LoadBuffer {

  //private member variables
  private:

    Memory* const memory;
    ReorderBuffer* const reorderBuffer;

    //index constants
    static const int entryFields = 4;
    static const int ADDRESS = 0;
    static const int DESTINATION_REGISTER = 1;
    static const int REORDER_BUFFER_INDEX = 2;
    static const int STEP = 3;

    //write buffer to hold inflight write operation info
    const int size;
    int** const buffer;
    int head;
    int tail;
    const int steps;

    ALUReservationStation* const aluReservationStation;

  //public functions
  public:

	LoadBuffer(Memory* const memory, ReorderBuffer* const reorderBuffer, const int size, const int steps,
      ALUReservationStation* const aluReservationStation);

    void addToBuffer(const int destinationRegister, const int address, const int reorderBufferIndex);

    void incrementSteps();

    void readIfReady();

    void flush();
};

#endif