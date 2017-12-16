using namespace std;
typedef struct instruction {
	int opcode;
	int operands[3];
} Instruction;


//return the number of instruction in the given file
int getNumOfInstructions(std::string inputFileName) {
    std::ifstream inputFile(inputFileName.c_str());
    if(inputFile.is_open()) {
        int i = 0;
        std::string line;
        while(getline(inputFile,line)) {
            i++;
        }
        return i;
    }
    else {
        cout << "Failed to read file.\n";
        return -1;
    }
}


//return an array of Instructions which contain the instructions in the given file
Instruction* getInstructions(std::string inputFileName, int numOfInstructions) {
    //open file
    std::ifstream inputFile(inputFileName.c_str());
    //if file is open then get the instruction information from each line
    if(inputFile.is_open()) {
        cout << "Reading instructions from " << inputFileName.c_str() << "\n";
        //allocating memory to array to store the instructions
        Instruction* instructions = new Instruction[numOfInstructions];

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
            while(pos != std::string::npos) {
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
        return instructions;
    }
    else {
        cout << "Failed to read file.\n";
        //if the file has not been opened then return a null pointer
        return NULL;
    }
}

void printInstruction(Instruction instruction) {
    switch(instruction.opcode) {
        case NOOP:
            cout << "NOOP" << endl;
            break;
        case ADD:
            cout << "ADD R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << "\n";
            break;
        case ADDI:
            cout << "ADDI R" << instruction.operands[0] << " R" << instruction.operands[1] << " " << instruction.operands[2] << "\n";
            break;
        case AND:
            cout << "AND R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << "\n";
            break;
        case MULT:
            cout << "MULT R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << "\n";
            break;
        case OR:
            cout << "OR R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << "\n";
            break;
        case SUB:
            cout << "SUB R" << instruction.operands[0] << " R" << instruction.operands[1] << " R" << instruction.operands[2] << "\n";
            break;
        case LW:
            cout << "LW R" << instruction.operands[0] << " " << instruction.operands[1] << "\n";
            break;
        case LWR:
            cout << "LWR R" << instruction.operands[0] << " R" << instruction.operands[1] << "\n";
            break;
        case SW:
            cout << "SW R" << instruction.operands[0] << " " << instruction.operands[1] << "\n";
            break;
        case SWR:
            cout << "SWR R" << instruction.operands[0] << " R" << instruction.operands[1] << "\n";
            break;
        case BEQ:
            cout << "BEQ R" << instruction.operands[0] << " R" << instruction.operands[1] << " " << instruction.operands[2] << "\n";
            break;
        case BGEZ:
            cout << "BGEZ R" << instruction.operands[0] << " " << instruction.operands[1] << "\n";
            break;
        case BGTZ:
            cout << "BGTZ R" << instruction.operands[0] << " " << instruction.operands[1] << "\n";
            break;
        case BLEZ:
            cout << "BLEZ R" << instruction.operands[0] << " " << instruction.operands[1] << "\n";
            break;
        case BLTZ:
            cout << "BLTZ R" << instruction.operands[0] << " " << instruction.operands[1] << "\n";
            break;
        case BNE:
            cout << "BNE R" << instruction.operands[0] << " R" << instruction.operands[1] << " " << instruction.operands[2] << "\n";
            break;
        case J:
            cout << "J " << instruction.operands[0] << "\n";
            break;
        case JR:
            cout << "JR R" << instruction.operands[0] << "\n";
            break;
        case END:
            cout << "END " << instruction.operands[0] << "\n";
            break;
        default:
            cout << "\n";
            break;
    }
}
