#include <iostream>
#include <fstream>
#include <string>	
#include <vector>
using namespace std;

int printOpcode(string op, ofstream *newfile) {
	int output = -1;
	if(op == "NOP") {
	    output = 0;
	}
	if(op == "ADD") {
		output = 1;
	}
	else if(op == "ADDI") {
		output = 2;
	}
	else if(op == "AND") {
		output = 3;
	}
	else if(op == "MULT") {
		output = 4;
	}
	else if(op == "OR") {
		output = 5;
	}
	else if(op == "SUB") {
		output = 6;
	}
	else if(op == "LW") {
        output = 7;
	}
	else if(op == "SW") {
        output = 8;
	}
	else if(op == "BEQ") {
		output = 9;
	}
	else if(op == "BGEZ") {
		output = 10;
	}
	else if(op == "BGTZ") {
		output = 11;
	}
	else if(op == "BLEZ") {
		output = 12;
	}
	else if(op == "BLTZ") {
		output = 13;
	}
	else if(op == "BNE") {
		output = 14;
	}
	else if(op == "J") {
		output = 15;
	}
	else if(op == "JR") {
		output = 16;
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
	if(extension != ".ass") {
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {

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
        
        /*
        //printing labels and corresponding instruction address
        for(int i = 0; i < labels.size(); i++) {
        	cout << labels.at(i);
        	cout << labelAddresses.at(i);
        	cout << "\n";
        }
        */

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
	return 0;
}