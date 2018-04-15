#include <iostream>
#include <fstream>
#include <string>	
#include <vector>
#include "../src/constants.h"
using namespace std;


int printOpcode(string op, ofstream *newfile) {
  int output = -1;
  if(op == "NOOP") {
    output = NOOP;
  }
  if(op == "ADD") {
    output = ADD;
  }
  else if(op == "ADDI") {
    output = ADDI;
  }
  else if(op == "AND") {
    output = AND;
  }
  else if(op == "MULT") {
    output = MULT;
  }
  else if(op == "OR") {
    output = OR;
  }
  else if(op == "SUB") {
    output = SUB;
  }
  else if(op == "LW") {
    output = LW;
  }
  else if(op == "LWR") {
    output = LWR;
  }
  else if(op == "SW") {
    output = SW;
  }
  else if(op == "SWR") {
    output = SWR;
  }
  else if(op == "BEQ") {
    output = BEQ;
  }
  else if(op == "BGEZ") {
    output = BGEZ;
  }
  else if(op == "BGTZ") {
    output = BGTZ;
  }
  else if(op == "BLEZ") {
    output = BLEZ;
  }
  else if(op == "BLTZ") {
    output = BLTZ;
  }
  else if(op == "BNE") {
    output = BNE;
  }
  else if(op == "J") {
    output = J;
  }
  else if(op == "JR") {
    output = JR;
  }
  else if(op == "HALT") {
    output = HALT;
  }
  *newfile << output;
  *newfile << " ";
  return output;
}

int findLabelIndex(vector<string> labels, string label) {
  int index = -1;
  for(int i = 0; i < labels.size(); i++) {
    if(labels.at(i) == label) {
      index = i;
      break;
    }
  }
  return index;
}

void printOperands(string operands, ofstream *newfile, vector<string> labels, vector<int> labelAddresses) {
  int pos = operands.find(" ");
  string operand;
  while(pos != string::npos) {
    operand = operands.substr(0,pos);
    //Check if the operand is a label, if so replace it with the corresponding address
    int labelIndex = findLabelIndex(labels, operand);
    if(labelIndex != -1) {
      int address = labelAddresses.at(labelIndex);
      *newfile << address << ' ';
    }
    else {
      //If operand is a register then just print the register number
      if(operand[0] == 'R') {
        operand = operand.substr(1, operand.size());
      }
      *newfile << operand << ' ';
    }
    operands = operands.substr(pos + 1, operands.size());
    pos = operands.find(" ");
  }
  operand = operands.substr(0,operands.size());
  //check if operand is a label and if so print the corresponding label address
  int labelIndex = findLabelIndex(labels, operand);
  if(labelIndex != -1) {
    int address = labelAddresses.at(labelIndex);
    *newfile << address;
  }
  else {
    //if the operand is a register the just print the register number
    if(operand[0] == 'R') {
     operand = operand.substr(1, operand.size());
    }
    *newfile << operand;
  }
}

string createOutputFileName(string inputFileName) {
  string name = inputFileName.substr(0, inputFileName.find('.'));
  string extension = ".mac";
  string outputFileName = name + extension;
  return outputFileName;
}

int checkValidInputFileName(string inputFileName) {
  int pos = inputFileName.find(".");
  if(pos == string::npos) {
    return 0;
  }
  string extension = inputFileName.substr(pos, inputFileName.size());
  if(extension != ".asm") {
    return 0;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  cout << "\n";

  //make sure an argument is given
  if(argc != 2) {
    cout << "incorrect arguments\n";
    return 1;
  }

  //get the input file name
  string inputFileName(argv[1]);
  if(checkValidInputFileName(inputFileName) == 0) {
    cout << "Invalid assembly file";
    return 1;
  }
  cout << "Input assembly file: " + inputFileName + '\n';

  //create the new file name
  string outputFileName = createOutputFileName(inputFileName);
  cout << "Output machine file: " + outputFileName + "\n";

  //the input and output files
  ifstream inputFile(inputFileName.c_str());
  ofstream outputFile(outputFileName.c_str());

  //vector of labels
  vector<string> labels;
  //vector of label addresses
  vector<int> labelAddresses;

  //begin writing machine to the output file
  string line;
  int lineNumber = 1;
  if(inputFile.is_open() && outputFile.is_open()) {
    while (getline(inputFile,line)) {
            
      //get the opcode
      string delimiter = " ";
      int delimiterPos = line.find(delimiter);
      string opstr = line.substr(0, delimiterPos);

      //check if is infact a label
      if(delimiterPos == string::npos && opstr.at(opstr.size() - 1) == ':') {
        //store the label and its corresponding instruction address
        string label = opstr.substr(0,opstr.size()-1);
        labels.push_back(label);
        labelAddresses.push_back(lineNumber - labelAddresses.size());
      }
      //otherwise write opcode and operands to file
      else {
        if(printOpcode(opstr, &outputFile) == -1) {
          printf("Assembler failed on line %d.\n", lineNumber);
          printf("Exiting assembler.\n");
          return 1;
        }

        //get the operands
        string operands = line.substr(delimiterPos + 1, line.size());
        printOperands(operands, &outputFile, labels, labelAddresses);
        //finish line
        outputFile << '\n';
      }
      lineNumber++;
    }

    //close the files
    inputFile.close();
    outputFile.close();

    //print success message
    cout << "Assembler was successful.\n";
  }
  else {
    //print failure message
    cout << "Unable to open file\n";
  }
  cout << "\n";
  return 0;
}