//===========================================
//included header file containing interface
#include "register_file.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cstring>

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
    printf("Register index %d is out of range.\n", i);
  }
  return registers[i];
}

void RegisterFile::setRegisterValue(int i, int val) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    printf("Register index %d is out of range.\n", i);
  }
  registers[i] = val;
}

int RegisterFile::getScoreBoardValue(int i ) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    printf("ScoreBoard index %d is out of range.\n", i);
  }
  return scoreBoard[i];
}

void RegisterFile::setScoreBoardValue(int i, int val) {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    printf("ScoreBoard index %d is our of range.\n", i);
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
  printf("Registers: ");
  int lengths[numOfArchitecturalRegisters];
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    lengths[i] = intLength(registers[i]);
  }
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    printf("R%d ", i);
    if(lengths[i] > 2) {
      for(int j = 2; j < lengths[i]; j++) {
        printf( " ");
      }
    }
  }
  printf("\n");

  //print register values
  printf("Values:    ");
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    if(lengths[i] == 1) {
      printf("%d  ", registers[i]);
    }
    else {
      printf("%d ", registers[i]);
    }
    if(i >= 10) {
      printf(" ");
    }
  }
  printf("\n");
}

void RegisterFile::printScoreBoard() {
  printf("ScoreBoard:\n");
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    printf("%d ", scoreBoard[i]);
  }
  printf("\n");
}
