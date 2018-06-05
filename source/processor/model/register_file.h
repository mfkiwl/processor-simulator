//===========================
//include guard
#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

//===========================
//class declaration
class RegisterFile {

  private:

    const int numOfArchitecturalRegisters;
    int* registers;
    int* scoreBoard;

  public: 
    RegisterFile(const int numOfRegisters);

    int getNumOfArchitecturalRegisters() const;

    void getAllRegisterValues(int* const copy) const;

    int getRegisterValue(const int i) const;

    void setRegisterValue(const int i, const int val);

    int getScoreBoardValue(const int i) const;

    void setScoreBoardValue(const int i, const int val);

    void resetScoreBoard();

    void printRegisters() const;

    void printScoreBoard() const;
};

#endif