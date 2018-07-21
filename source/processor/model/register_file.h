//===========================
//include guard
#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

//===========================
//class declaration
class RegisterFile {

  //private member variables
  private:

    const int numOfArchitecturalRegisters;
    const int numOfPhysicalRegisters;
    //mapping table of the archtictural registers to the physical registers
    int* const renameTable;
    //the values in the physical registers
    int* const physicalRegisters;
    //tells whether each physical registers is free to be used or not
    int* const freeList;
    //tells us whether each physical register is in the process of being written to
    int* const scoreBoard;

  //public functions
  public:

    RegisterFile(const int numOfRegisters);

    void resetScoreBoard();

    void printRegisters() const;

    void printScoreBoard() const;

    int findFreePhysicalRegister() const;

    void freeRegister(const int i);

    void useRegister(const int i);

  //getters and setters
  public:

    int getNumOfArchitecturalRegisters() const;

    void getRenameTable(int* const copy) const;

    void getAllRegisterValues(int* const copy) const;

    int getRegisterValue(const int i) const;

    void setRegisterValue(const int i, const int val);

    int getScoreBoardValue(const int i) const;

    void setScoreBoardValue(const int i, const int val);
};

#endif