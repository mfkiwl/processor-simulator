//===========================
//include guard
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

//=================================
// included dependencies
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "constants.h"

using namespace std;

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
    void loadNumOfInstructions(std::string inputFileName) {
        std::ifstream inputFile(inputFileName.c_str());
        if(inputFile.is_open()) {
            int i = 0;
            std::string line;
            while(getline(inputFile,line)) {
                i++;
            }
            numOfInstructions = i;
        }
        else {
            cout << "Failed to read file.\n";
            numOfInstructions = -1;
        }
    }

    //return an array of Instructions which contain the instructions in the given file
    void loadInstructions(std::string inputFileName) {
        //open file
        std::ifstream inputFile(inputFileName.c_str());
        //if file is open then get the instruction information from each line
        if(inputFile.is_open()) {
            cout << "Reading instructions from " << inputFileName.c_str() << "\n";
            //allocating memory to array to store the instructions
            instructions = new Instruction[numOfInstructions];

            //interating through each instruction
            std::string line;
            for(int i = 0; i < numOfInstructions; i++) {
                getline(inputFile,line);

                //get the opcode
                int pos = line.find(" ");
                std::string opcodestr = line.substr(0,pos);
                line = line.substr(pos + 1, line.size());
                int opcode = atoi(opcodestr.c_str());
                instructions[i].opcode = opcode;
            
                //get the operands
                pos = line.find(" ");
                int j = 0;
                while(pos != (int) std::string::npos) {
                    //get operand
                    std::string operandstr = line.substr(0, pos);
                    int operand = atoi(operandstr.c_str());
                    instructions[i].operands[j] = operand;

                    //update values
                    line = line.substr(pos + 1, line.size());
                    pos = line.find(" ");
                    j++;
                }
                //get the final operand
                std::string operandstr = line.substr(0, line.size());
                int operand = atoi(operandstr.c_str());
                instructions[i].operands[j] = operand;
            }
            //return the instructions array
        }
        else {
            cout << "Failed to read file.\n";
            //if the file has not been opened then return a null pointer
            instructions = NULL;
        }
    }

};

void printInstruction(Instruction instruction);

#endif