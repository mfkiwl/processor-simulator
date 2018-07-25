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
  numOfArchitecturalRegisters(numOfRegisters),
  numOfPhysicalRegisters(100),
  renameTable(new int[numOfArchitecturalRegisters]),
  physicalRegisters(new int[numOfPhysicalRegisters]),
  freeList(new int[numOfPhysicalRegisters]),
  scoreBoard(new int[numOfPhysicalRegisters]),
  rollbackRenameTable(new int[numOfArchitecturalRegisters])
{
  //set all the physical register values to 0
  //set all physical registers to be free
  //set all scoreboard values to 1
  for(int i = 0; i < numOfPhysicalRegisters; i++) {
    physicalRegisters[i] = 0;
    freeList[i] = 1;
    scoreBoard[i] = 1;
  }
  //map each architectural register to a unique physical register
  //set the latest architecural register values to 0
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    renameTable[i] = i;
    rollbackRenameTable[i] = i;
    freeList[i] = 0;
  }
}

void RegisterFile::resetScoreBoard() {
  for(int i = 0; i < numOfPhysicalRegisters; i++) {
    scoreBoard[i] = 1;
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
  int lengths[numOfArchitecturalRegisters];
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    lengths[i] = intLength(physicalRegisters[renameTable[i]]);
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
      printf("%d  ", physicalRegisters[renameTable[i]]);
    }
    else {
      printf("%d ", physicalRegisters[renameTable[i]]);
    }
    if(i >= 10) {
      printf(" ");
    }
  }
  printf("\n");
}

void RegisterFile::printScoreBoard() const {
  printf("ScoreBoard:\n");
  for(int i = 0; i < numOfPhysicalRegisters; i++) {
    printf("%d ", scoreBoard[i]);
  }
  printf("\n");
}

int RegisterFile::findFreePhysicalRegister() const {
  for(int i = 0; i < numOfPhysicalRegisters; i++) {
    if(freeList[i] == 1) {
      return i;
    }
  }
  return -1;
}

void RegisterFile::freePhysicalRegister(const int i) {
  if(i < 0 || i > numOfPhysicalRegisters) {
    printf("Physical register index %d is out of range when trying to free register.\n", i);
  }
  freeList[i] = 1;
  physicalRegisters[i] = 0;
}

void RegisterFile::flush() {
  //rollback the rename table
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    renameTable[i] = rollbackRenameTable[i];
  }
  //free all necessary registers
  for(int i = 0; i < numOfPhysicalRegisters; i++) {
    freeList[i] = 1;
  }
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    freeList[renameTable[i]] = 0;
  }
  //reset the scoreboard
  for(int i = 0; i < numOfPhysicalRegisters; i++) {
    scoreBoard[i] = 1;
  }
}

void RegisterFile::usePhysicalRegister(const int i) {
  if(i < 0 || i > numOfPhysicalRegisters) {
    printf("Physical register index %d is out of range when trying to use register.\n", i);
  }
  freeList[i] = 0;
}

//================================================================================
//getters and setters

int RegisterFile::getNumOfArchitecturalRegisters() const {
  return numOfArchitecturalRegisters;
}

void RegisterFile::getRenameTable(int* const copy) const {
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    copy[i] = renameTable[i];
  }
}

void RegisterFile::getArchitecturalRegisterValues(int* const copy) const {
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    copy[i] = physicalRegisters[renameTable[i]];
  }
}

void RegisterFile::getLatestArchitecturalRegisterValues(int* const copy) const {
  for(int i = 0; i < numOfArchitecturalRegisters; i++) {
    copy[i] = physicalRegisters[rollbackRenameTable[i]];
  }
}

int RegisterFile::getPhysicalRegisterValue(const int i) const {
  if(i < 0 || i >= numOfPhysicalRegisters) {
    printf("Physical register index %d is out of range when trying to get register value.\n", i);
  }
  return physicalRegisters[i];
}

void RegisterFile::setPhysicalRegisterValue(const int i, const int val) {
  if(i < 0 || i >= numOfPhysicalRegisters) {
    printf("Physical register index %d is out of range when trying to set register value.\n", i);
  }
  physicalRegisters[i] = val;
}

int RegisterFile::getScoreBoardValue(const int i) const {
  if(i < 0 || i >= numOfPhysicalRegisters) {
    printf("ScoreBoard index %d is out of range when trying to get scoreBoard value.\n", i);
  }
  return scoreBoard[i];
}

void RegisterFile::setScoreBoardValue(const int i, const int val) {
  if(i < 0 || i >= numOfPhysicalRegisters) {
    printf("ScoreBoard index %d is our of range when trying to set scoreBoard value.\n", i);
  }
  scoreBoard[i] = val;
}

void RegisterFile::setArchitecturalRegisterMapping(const int i, const int val) const {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    printf("Architectural register index %d is out of range when trying to set renameTable mapping.\n", i);
  }
  if(val < 0 || val >= numOfPhysicalRegisters) {
    printf("Physical register index %d is out of range when trying to set renameTable mapping.\n", i);
  }
  renameTable[i] = val;
}

int RegisterFile::getArchitecturalRegisterMapping(const int i) const {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    printf("Architectural register index %d is out of range when trying to get renameTable mapping.\n", i);
  }
  return renameTable[i];
}

void RegisterFile::setRollbackRenameTableMapping(const int i, const int val) const {
  if(i < 0 || i >= numOfArchitecturalRegisters) {
    printf("Architectural register index %d is out of range when trying to set rollbackRenameTable mapping.\n", i);
  }
  rollbackRenameTable[i] = val;
}