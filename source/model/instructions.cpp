#include "instructions.h"

Instructions::Instructions(std::string inputFileName) {
  loadNumOfInstructions(inputFileName);
  loadInstructions(inputFileName);
}

int Instructions::getNumOfInstructions() {
  return numOfInstructions;
}

Instruction* Instructions::getInstructions() {
  return instructions;
}

Instruction Instructions::at(int i) {
  return instructions[i];
}

void printInstruction(Instruction instruction) {
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