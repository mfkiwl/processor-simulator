#include <iostream>
#include <fstream>
#include <string>	
using namespace std;

int printOpcode(string op, ofstream *newfile) {
	if(op == "ADD") {
		*newfile << 0;
	}
	else if(op == "ADDI") {
		*newfile << 1;
	}
	else if(op == "SUB") {
		*newfile << 2;
	}
	else if(op == "B") {
		*newfile << 3;
	}
	else if(op == "LD") {
		*newfile << 4;
	}
	else if(op == "STR") {
		*newfile << 5;
	}
	*newfile << " ";
}

void printOperands(string operands, ofstream *newfile) {
	int pos = operands.find(" ");
	string operand;
	while(pos != string::npos) {
		operand = operands.substr(0,pos);
		if(operand[0] == 'R') {
			operand = operand.substr(1, operand.size());
		}
		*newfile << operand << ' ';
		operands = operands.substr(pos + 1, operands.size());
		pos = operands.find(" ");
	}
	*newfile << operands;
}

int main(void) {
	string line;
	ifstream myfile("memory.ass");
	ofstream newfile("memory.mac");
	if(myfile.is_open() && newfile.is_open()) {
		while (getline(myfile,line)) {
            
            //get the opcode
			string delimiter = " ";
			int delimiterPos = line.find(delimiter);
			string opstr = line.substr(0, delimiterPos);
			printOpcode(opstr, &newfile);

			//get the operands
			string operands = line.substr(delimiterPos + 1, line.size());
			printOperands(operands, &newfile);

            //finish line
			newfile << '\n';

		}
		myfile.close();
		newfile.close();
		cout << "new file created.\n";
	}
	else {
		cout << "Unable to open file";
	}
	return 0;
}