//===========================
//include guard
#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

//===========================
//class declaration
using namespace std;
class RegisterFile {

    int numOfArchitecturalRegisters;
    int* registers;
    int* scoreBoard;

  public: 
    RegisterFile();

    int getNumOfArchitecturalRegisters();

    int getRegisterValue(int i);

    void setRegisterValue(int i, int val);

    int getScoreBoardValue(int i );

    void setScoreBoardValue(int i, int val);

    void resetScoreBoard();

    void printRegisters();

    void printScoreBoard();
};

#endif