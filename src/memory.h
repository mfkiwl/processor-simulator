#ifndef MEMORY_H
#define MEMROY_H

class Memory {

    //the number of bytes the memory has
    int size;
    int* memory;

  public:
    Memory(int size) : size(size) {
      memory = new int[size];
      for(int i = 0; i < size; i++) {
        memory[i] = 0;
      }
    }
    
    int loadFromMemory(int address) {
      if(address < 0 || address > size - 1) {
        cout << "Register index " << address << " is out of bounds.\n";
      }
      return memory[address];
    }

    void storeInMemory(int address, int value) {
      if(address < 0 || address > size - 1) {
        cout << "Register index " << address << " is out of bounds.\n";
      }
      memory[address] = value;
    }

    void print() {
      cout << "Memory:" << endl;
      for(int i = 0; i < size; i++) {
        cout << memory[i] << " ";
      }
      cout << endl;
    }
};

#endif