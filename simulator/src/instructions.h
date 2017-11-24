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
        printf("Failed to read file.\n");
        return -1;
    }
}


//return an array of Instructions which contain the instructions in the given file
Instruction* getInstructions(std::string inputFileName, int numOfInstructions) {
    //open file
    std::ifstream inputFile(inputFileName.c_str());
    //if file is open then get the instruction information from each line
    if(inputFile.is_open()) {
        printf("Reading instructions from %s\n", inputFileName.c_str());
        //allocating memory to array to store the instructions
        Instruction* instructions = (Instruction*) malloc(sizeof(Instruction) * numOfInstructions);

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
        printf("Failed to read file.\n");
        //if the file has not been opened then return a null pointer
        return NULL;
    }
}

void printInstruction(Instruction instruction) {
            switch(instruction.opcode) {
                case 1:
                    printf("ADD R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
                    break;
                case 2:
                    printf("ADDI R%d R%d %d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
                    break;
                case 3:
                    printf("SUB R%d R%d R%d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
                    break;
                case 4:
                    printf("B %d\n", instruction.operands[0]);
                    break;
                case 5:
                    printf("LD R%d R%d %d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
                    break;
                case 6:
                    printf("STR R%d R%d %d\n", instruction.operands[0], instruction.operands[1], instruction.operands[2]);
                    break;
                default:
                    printf("\n");
                    break;
            }
        }