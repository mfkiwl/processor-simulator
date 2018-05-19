//===========================================
//included header file containing interface
#include "register_file.h"

//===========================================
// included dependencies
#include <iostream>
#include <math.h>
#include <stdlib.h>

//===========================================
//class implementation

using namespace std;

RegisterFile::RegisterFile() :   
  numOfArchitecturalRegisters(16) 
{
  registers = new int[numOfArchitecturalRegisters];
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    registers[i] = 0;
  }
  scoreBoard = new int[numOfArchitecturalRegisters];
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    scoreBoard[i] = 1;
  }
}

int RegisterFile::getNumOfArchitecturalRegisters() {
  return numOfArchitecturalRegisters;
}

int RegisterFile::getRegisterValue(int i) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    cout << "Register index " << i << " is out of range.\n";
  }
  return registers[i];
}

void RegisterFile::setRegisterValue(int i, int val) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    cout << "Register index " << i << " is out of range.\n";
  }
  registers[i] = val;
}

int RegisterFile::getScoreBoardValue(int i ) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    cout << "ScoreBoard index " << i << " is out of range.\n";
  }
  return scoreBoard[i];
}

void RegisterFile::setScoreBoardValue(int i, int val) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    cout << "ScoreBoard index " << i << " is our of range.\n";
  }
  scoreBoard[i] = val;
}

void RegisterFile::resetScoreBoard() {
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    scoreBoard[i] = 1;
  }
}

int intLength(int n) {
  if(n == 0) {
    return 1;
  } else {
    int nDigits = floor(log10(abs(n))) + 1;
    if(n < 0) {
      nDigits++;
    }
    return nDigits;
  }
}

void RegisterFile::printRegisters() {

  //print register names
  cout << "Registers: ";
  int lengths[numOfArchitecturalRegisters];
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    lengths[i] = intLength(registers[i]);
  }
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    cout << "R" << i << " ";
    if(lengths[i] > 2) {
      for(int j = 2; j < lengths[i]; j++) {
        cout << " ";
      }
    }
  }
  cout << endl;

  //print register values
  cout << "Values:    ";
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    if(lengths[i] == 1) {
      cout << registers[i] << "  ";
    }
    else {
      cout << registers[i] << " ";
    }
    if(i >= 10) {
      cout << " ";
    }
  }
  cout << endl;
}

void RegisterFile::printScoreBoard() {
  cout << "ScoreBoard:" << endl;
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    cout << scoreBoard[i] << " ";
  }
  cout << endl;
}
