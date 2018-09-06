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

//====================================================================================
//public functions

RegisterFile::RegisterFile(const int numOfRegisters) :   
  numOfRegisters(numOfRegisters),
  renameTable(new int[numOfRegisters]),
  robMapping(new bool[numOfRegisters]),
  registers(new int[numOfRegisters])
{
  //initialise the arrays
  for(int i = 0; i < numOfRegisters; i++) {
    renameTable[i] = i;
    robMapping[i] = false;
    registers[i] = 0;
  }
}

int intLength(const int n) {
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

void RegisterFile::printRegisters() const {

  //print register names
  printf("Registers: ");
  int lengths[numOfRegisters];
  for(int i = 0; i < numOfRegisters; i++) {
    lengths[i] = intLength(registers[i]);
  }
  for(int i = 0; i < numOfRegisters; i++) {
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
  for(int i = 0; i < numOfRegisters; i++) {
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

void RegisterFile::flush() {
  for(int i = 0; i < numOfRegisters; i++) {
    renameTable[i] = i;
    robMapping[i] = false;
    registers[i] = 0;
  }
}

//================================================================================
//getters and setters

int RegisterFile::getNumOfRegisters() const {
  return numOfRegisters;
}

void RegisterFile::getRenameTable(int copy[]) const {
  for(int i = 0; i < numOfRegisters; i++) {
    copy[i] = renameTable[i];
  }
}

void RegisterFile::getRegisterValues(int copy[]) const {
  for(int i = 0; i < numOfRegisters; i++) {
    copy[i] = registers[i];
  }
}

int RegisterFile::getRegisterValue(const int i) const {
  if(i < 0 || i >= numOfRegisters) {
    printf("Register index %d is out of range when trying to get register value.\n", i);
  }
  return registers[i];
}

void RegisterFile::setRegisterValue(const int i, const int val) {
  if(i < 0 || i >= numOfRegisters) {
    printf("Register index %d is out of range when trying to set register value.\n", i);
  }
  registers[i] = val;
}

void RegisterFile::setMappingToRegister(const int i) {
  if(i < 0 || i >= numOfRegisters) {
    printf("Index %d is out of range when trying to set register mapping.\n", i);
  }
  renameTable[i] = i;
  robMapping[i] = false;
}

void RegisterFile::setMappingToRobEntry(const int i, const int map) {
  if(i < 0 || i >= numOfRegisters) {
    printf("Index %d is out of range when trying to set ROB entry mapping", i);
  }
  renameTable[i] = map;
  robMapping[i] = true;
}

bool RegisterFile::isRobMapping(const int i) const {
  if(i < 0 || i >= numOfRegisters) {
    printf("Register index %d is out of range when trying to get rob mapping.\n", i);
  }
  return robMapping[i];
}

int RegisterFile::getRegisterMapping(const int i) const {
  if(i < 0 || i >= numOfRegisters) {
    printf("Register index %d is out of range when trying to get renameTable mapping.\n", i);
  }
  return renameTable[i];
}

void RegisterFile::getRobMapping(bool copy[]) const {
  for(int i = 0; i < numOfRegisters; i++) {
    copy[i] = robMapping[i];
  }
}