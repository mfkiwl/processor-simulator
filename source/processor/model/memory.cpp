//===========================================
//included header file containing interface
#include "memory.h"

//===========================================
// included dependencies
#include <stdio.h>

//===========================================
//class implementation

Memory::Memory(const int size) : 
  size(size), 
  memory(new int[size]) 
{
  //set all memory values to 0
  for(int i = 0; i < size; i++) {
    memory[i] = 0;
  }
}

void Memory::getAllMemoryValues(int* const copy) const {
  for(int i = 0; i < size; i++) {
    copy[i] = memory[i];
  }
}

int Memory::loadFromMemory(const int address) const {
  if(address < 0 || address > size - 1) {
    printf("Memory address %d is out of bounds.\n", address);
  }
  return memory[address];
}

void Memory::storeInMemory(const int address, const int value) {
  if(address < 0 || address > size - 1) {
    printf("Memory address %d is out of bounds.\n", address);
  }
  memory[address] = value;
}

void Memory::print() const {
  printf("Memory:\n");
  for(int i = 0; i < size; i++) {
    printf("%d ", memory[i]);
  }
  printf("\n");
}