//===========================
//include guard
#ifndef MEMORY_H
#define MEMORY_H

//===========================
//class declaration
class Memory {

  private:

    //the number of bytes the memory has
    const int size;
    int* const memory;

  public:
    Memory(const int size);

    void getAllMemoryValues(int* const copy) const;

    int loadFromMemory(const int address) const;

    void storeInMemory(const int address, const int value);

    void print() const;
};

#endif