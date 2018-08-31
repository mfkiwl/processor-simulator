//===========================================
//included header file containing interface
#include "instructions.h"

//===========================================
// included dependencies
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "constants.h"

//===========================================
//Implementation

using namespace std;

Instructions::Instructions(const std::string inputFileName) {
  loadNumOfInstructions(inputFileName);
  loadInstructions(inputFileName);
}

int Instructions::getNumOfInstructions() const {
  return numOfInstructions;
}

Instruction* Instructions::getInstructions() const {
  return instructions;
}

Instruction Instructions::at(const int i) const {
  if(i < 0 || i >= numOfInstructions) {
    printf("Instructions index %d is out of range.\n", i);
  }
  return instructions[i];
}

//return the number of instruction in the given file
void Instructions::loadNumOfInstructions(const std::string inputFileName) {
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
    printf("Failed to read file.\n");
    numOfInstructions = -1;
  }
}

void Instructions::loadInstructions(const std::string inputFileName) {
  //open file
  std::ifstream inputFile(inputFileName.c_str());
  //if file is open then get the instruction information from each line
  if(inputFile.is_open()) {
    printf("Reading instructions from %s\n", inputFileName.c_str());
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
  }
  else {
    printf("Failed to read file.\n");
    //if the file has not been opened then return a null pointer
    instructions = NULL;
  }
}

void printInstruction(const Instruction instruction) {
  switch(instruction.opcode) {
    case NOOP:
      printf("NOOP\n");
      break;
    case ADD:
      printf("ADD R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case ADDI:
      printf("ADDI R%d R%d %d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case AND:
      printf("AND R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case MULT:
      printf("MULT R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case OR:
      printf("OR R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case SUB:
      printf("SUB R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case LW:
      printf("LW R%d %d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case LWR:
      printf("LWR R%d R%d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case SW:
      printf("SW R%d %d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case SWR:
      printf("SWR R%d R%d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case BEQ:
      printf("BEQ R%d R%d %d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case BGEZ:
      printf("BGEZ R%d %d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case BGTZ:
      printf("BGTZ R%d %d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case BLEZ:
      printf("BLEZ R%d %d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case BLTZ:
      printf("BLTZ R%d %d\n", instruction.operands[0], instruction.operands[1]);
      break;
    case BNE:
      printf("BNE R%d R%d %d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
      break;
    case J:
      printf("J %d\n", instruction.operands[0]);
      break;
    case JR:
      printf("JR R%d\n", instruction.operands[0]);
      break;
    case HALT:
      printf("HALT\n");
      break;
    default:
      printf("\n");
      break;
  }
}

std::string intToString(const int i) {
  std::stringstream ss;
  ss << i;
  std::string output = ss.str();
  return output;
}

std::string floatToString(const float i) {
  std::ostringstream ss;
  ss << i;
  std::string output = ss.str();
  return output;
}

std::string opcodeToString(const int opcode) {
  switch(opcode) {
    case ADD:
      return "ADD";
      break;
    case ADDI:
      return "ADDI";
      break;
    case AND:
      return "AND";
      break;
    case MULT:
      return "MULT";
      break;
    case OR:
      return "OR";
      break;
    case SUB:
      return "SUB";
      break;
    case LW:
      return "LW";
      break;
    case LWR:
      return "LWR";
      break;
    case SW:
      return "SW";
      break;
    case SWR:
      return "SWR";
      break;
    case BEQ:
      return "BEQ";
      break;
    case BGEZ:
      return "BGEZ";
      break;
    case BGTZ:
      return "BGTZ";
      break;
    case BLEZ:
      return "BLEZ";
      break;
    case BLTZ:
      return "BLTZ";
      break;
    case BNE:
      return "BNE";
      break;
    case J:
      return "J";
      break;
    case JR:
      return "JR";
      break;
    case HALT:
      return "HALT";
      break;
    default:
      return "";
      break;
  }
}

std::string instructionToString(const Instruction instruction) {
  switch(instruction.opcode) {
    case NOOP:
      return "NOOP";
      break;
    case ADD:
      return "ADD R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " R" + intToString(instruction.operands[2]);
      break;
    case ADDI:
      return "ADDI R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " " + intToString(instruction.operands[2]);
      break;
    case AND:
      return "AND R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " R" + intToString(instruction.operands[2]);
      break;
    case MULT:
      return "MULT R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " R" + intToString(instruction.operands[2]);
      break;
    case OR:
      return "OR R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " R" + intToString(instruction.operands[2]);
      break;
    case SUB:
      return "SUB R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " R" + intToString(instruction.operands[2]);
      break;
    case LW:
      return "LW R" + intToString(instruction.operands[0]) + " " + intToString(instruction.operands[1]);
      break;
    case LWR:
      return "LWR R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]);
      break;
    case SW:
      return "SW R" + intToString(instruction.operands[0]) + " " + intToString(instruction.operands[1]);
      break;
    case SWR:
      return "SWR R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]);
      break;
    case BEQ:
      return "BEQ R" +  intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " " +  intToString(instruction.operands[2]);
      break;
    case BGEZ:
      return "BGEZ R" + intToString(instruction.operands[0]) + " " + intToString(instruction.operands[1]);
      break;
    case BGTZ:
      return "BGTZ R" + intToString(instruction.operands[0]) + " " + intToString(instruction.operands[1]);
      break;
    case BLEZ:
      return "BLEZ R" + intToString(instruction.operands[0]) + " " + intToString(instruction.operands[1]);
      break;
    case BLTZ:
      return "BLTZ R" + intToString(instruction.operands[0]) + " " + intToString(instruction.operands[1]);
      break;
    case BNE:
      return "BNE R" + intToString(instruction.operands[0]) + " R" + intToString(instruction.operands[1]) + 
      " " + intToString(instruction.operands[2]);
      break;
    case J:
      return "J " + intToString(instruction.operands[0]);
      break;
    case JR:
      return "JR R" + intToString(instruction.operands[0]) + "\n";
      break;
    case HALT:
      return "HALT";
      break;
    default:
      return "";
      break;
  }
}