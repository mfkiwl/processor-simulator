class ALU {

    //forward components
    RegisterFile* registerFile;

    //decoded instruction
    int opcode;
    int operands[3];

    //instruction result
    int destinationRegister;
    int result;

    //no of instruction executed by the processor
    int* noOfInstructionsExecuted;

    //for debugging purposes
    Instruction nextInstruction;
    Instruction currentInstruction;

    public:
        ALU(RegisterFile* registerFile, int* noOfInstructionsExecuted) : 
            registerFile(registerFile), 
            opcode(0),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            destinationRegister(-1),
            result(0)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {
                //execute the instruction
                destinationRegister = operands[0];
                switch(opcode) {
                    case ADD:
                    case ADDI:
                        result = operands[1] + operands[2];
                        break;
                    case AND:
                        result = operands[1] && operands[2];
                        break;
                    case MULT:
                        result = operands[1] * operands[2];
                        break;
                    case OR:
                        result = operands[1] || operands[2];
                        break;
                    case SUB:
                        result = operands[1] - operands[2];
                        break;
                }
            }
            currentInstruction = nextInstruction;
            //reset inputs
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void writeback() {
            if(destinationRegister != -1) {
                //write the result to the output register
                registerFile->setRegisterValue(destinationRegister, result);
                //Set the scoreboard of the destination register to 1
                registerFile->setScoreboardValue(destinationRegister, 1);

                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;

                //print the instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(currentInstruction);

                //reset variables
                destinationRegister = -1;
                currentInstruction = (Instruction) {0,0,0,0};
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int x[3]) {
            for(int i = 0; i < 3; i++) {
                operands[i] = x[i];
            }
        }

        void setNextInstruction(Instruction i) {
            nextInstruction = i;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //reset debug instruciton
            nextInstruction = (Instruction) {0,0,0,0};
            currentInstruction = (Instruction) {0,0,0,0};
        }
};
