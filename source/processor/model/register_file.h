//===========================
//include guard
#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

//===========================
//class declaration
class RegisterFile {

  //private member variables
  private:

    //number of physical registers
    const int numOfRegisters;
    //mapping table of the archtictural registers to the physical registers
    int* const renameTable;
    //indicating whether or not the mapping is to a reorder buffer entry
    bool* const robMapping;
    //the values in the physical registers
    int* const registers;

  //public functions
  public:

    RegisterFile(const int numOfRegisters);

    void printRegisters() const;

    void flush();

    void usePhysicalRegister(const int i);

  //getters and setters
  public:

    int getNumOfRegisters() const;

    void getRenameTable(int copy[]) const;

    void getRegisterValues(int copy[]) const;

    int getRegisterValue(const int i) const;

    void setRegisterValue(const int i, const int val);

    void setMappingToRegister(const int i);

    void setMappingToRobEntry(const int i, const int map);

    bool isRobMapping(const int i) const;

    int getRegisterMapping(const int i) const;

    void getRobMapping(bool copy[]) const;

};

#endif