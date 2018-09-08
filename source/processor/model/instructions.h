//================================
//include guard
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

//=================================
//included dependencies
#include <string>
#include "constants.h"

//=================================
//interface

struct Instruction {
	int opcode;
	int operands[numOfOperands];
};

class Instructions {

  private:

    int numOfInstructions;
    Instruction* instructions;

  public:
    Instructions(const std::string inputFileName);

    int getNumOfInstructions() const;

    Instruction* getInstructions() const;

    Instruction at(const int i) const;

    private:

    //return the number of instruction in the given file
    void loadNumOfInstructions(const std::string inputFileName);

    //return an array of Instructions which contain the instructions in the given file
    void loadInstructions(const std::string inputFileName);

};

void printInstruction(const Instruction instruction);

std::string intToString(const int i);

std::string floatToString(const float i);

std::string operandsToString(const int operands[numOfOperands], const OperandType operandTypes[numOfOperands]);

std::string operandToString(const int operand, const OperandType operandType);

std::string opcodeToString(const int opcode);

std::string instructionToString(const Instruction instruction);

#endif