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
    bool* const scoreBoard;

    int* const rollbackRenameTable;

  //public functions
  public:

    RegisterFile(const int numOfArchitecturalRegisters, const int numOfPhysicalRegisters);

    void resetScoreBoard();

    void printRegisters() const;

    void printScoreBoard() const;

    int findFreePhysicalRegister() const;

    void freePhysicalRegister(const int i);

    bool freePhysicalRegisterAvailable() const;

    void flush();

    void usePhysicalRegister(const int i);

  //getters and setters
  public:

    int getNumOfArchitecturalRegisters() const;

    void getRenameTable(int copy[]) const;

    void getLatestArchitecturalRegisterValues(int copy[]) const;

    void getArchitecturalRegisterValues(int copy[]) const;

    int getPhysicalRegisterValue(const int i) const;

    void setPhysicalRegisterValue(const int i, const int val);

    bool getScoreBoardValue(const int i) const;

    void setScoreBoardValue(const int i, const bool val);

    void setArchitecturalRegisterMapping(const int i, const int val) const;

    int getArchitecturalRegisterMapping(const int i) const;

    void setRollbackRenameTableMapping(const int i, const int val) const;

};

#endif