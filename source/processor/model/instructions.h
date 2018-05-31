//================================
//include guard
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

//=================================
//included dependencies
#include <string>

//=================================
//interface

struct Instruction {
	int opcode;
	int operands[3];
};

class Instructions {
    int numOfInstructions;
    Instruction* instructions;

public:
    Instructions(std::string inputFileName);

    int getNumOfInstructions();

    Instruction* getInstructions();

    Instruction at(int i);

    private:

    //return the number of instruction in the given file
    void loadNumOfInstructions(std::string inputFileName);

    //return an array of Instructions which contain the instructions in the given file
    void loadInstructions(std::string inputFileName);

};

void printInstruction(Instruction instruction);

#endif