//===========================================
//included header file containing interface
#include "memory.h"

//===========================================
// included dependencies
#include <iostream>

//===========================================
//class implementation

Memory::Memory(int size) : size(size) {
  memory = new int[size];
  for(int i = 0; i < size; i++) {
    memory[i] = 0;
  }
}

int Memory::loadFromMemory(int address) {
  if(address < 0 || address > size - 1) {
    std::cout << "Register index " << address << " is out of bounds.\n";
  }
  return memory[address];
}

void Memory::storeInMemory(int address, int value) {
  if(address < 0 || address > size - 1) {
    std::cout << "Register index " << address << " is out of bounds.\n";
  }
  memory[address] = value;
}

void Memory::print() {
  std::cout << "Memory:" << std::endl;
  for(int i = 0; i < size; i++) {
    std::cout << memory[i] << " ";
  }
  std::cout << std::endl;
}