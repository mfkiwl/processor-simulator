//===========================
//include guard
#ifndef MEMORY_H
#define MEMORY_H

//===========================
//class declaration
class Memory {

    //the number of bytes the memory has
    int size;
    int* memory;

  public:
    Memory(int size);

    void getAllMemoryValues(int* copy);

    int loadFromMemory(int address);

    void storeInMemory(int address, int value);

    void print();
};

#endif