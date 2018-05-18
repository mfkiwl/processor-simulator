#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#include <fstream>
#include <stdlib.h>


using namespace std;
struct Instruction {
	int opcode;
	int operands[3];
};

class Instructions {
    int numOfInstructions;
    Instruction* instructions;

public:
    Instructions(std::string inputFileName) {
        loadNumOfInstructions(inputFileName);
        loadInstructions(inputFileName);
    }

    int getNumOfInstructions() {
        return numOfInstructions;
    }

    Instruction* getInstructions() {
        return instructions;
    }

    Instruction at(int i) {
        return instructions[i];
    }

    static void printInstruction(Instruction instruction) {
        switch(instruction.opcode) {
            case NOOP:
                cout << "NOOP" << endl;
                break;
            case ADD:
                cout << "ADD R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << endl;
                break;
            case ADDI:
                cout << "ADDI R" << instruction.operands[0] << " R" << instruction.operands[1] << " " << instruction.operands[2] << endl;
                break;
            case AND:
                cout << "AND R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << endl;
                break;
            case MULT:
                cout << "MULT R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << endl;
                break;
            case OR:
                cout << "OR R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << endl;
                break;
            case SUB:
                cout << "SUB R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << endl;
                break;
            case LW:
                cout << "LW R" << instruction.operands[0] << " " << instruction.operands[1] << endl;
                break;
            case LWR:
                cout << "LWR R" << instruction.operands[0] << " R" << instruction.operands[1] << endl;
                break;
            case SW:
                cout << "SW R" << instruction.operands[0] << " " << instruction.operands[1] << endl;
                break;
            case SWR:
                cout << "SWR R" << instruction.operands[0] << " R" << instruction.operands[1] << endl;
                break;
            case BEQ:
                cout << "BEQ R" << instruction.operands[0] << " R" << instruction.operands[1] << " " << instruction.operands[2] << endl;
                break;
            case BGEZ:
                cout << "BGEZ R" << instruction.operands[0] << " " << instruction.operands[1] << endl;
                break;
            case BGTZ:
                cout << "BGTZ R" << instruction.operands[0] << " " << instruction.operands[1] << endl;
                break;
            case BLEZ:
                cout << "BLEZ R" << instruction.operands[0] << " " << instruction.operands[1] << endl;
                break;
            case BLTZ:
                cout << "BLTZ R" << instruction.operands[0] << " " << instruction.operands[1] << endl;
                break;
            case BNE:
                cout << "BNE R" << instruction.operands[0] << " R" << instruction.operands[1] << " " << instruction.operands[2] << endl;
                break;
            case J:
                cout << "J " << instruction.operands[0] << endl;
                break;
            case JR:
                cout << "JR R" << instruction.operands[0] << endl;
                break;
            case HALT:
                cout << "HALT" << endl;
                break;
            default:
                cout << "\n";
                break;
        }
    }

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

#endif