class ALU {

    //forward components
    RegisterFile* registerFile;

    //decoded instruction
    int opcode;
    int* operands;

    //instruction result
    int result;

    //no of instruction executed by the processor
    int* noOfInstructionsExecuted;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        ALU(RegisterFile* registerFile, int* noOfInstructionsExecuted) : 
            registerFile(registerFile), 
            opcode(0),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            result(0)
        {}

        void execute() {
            if(opcode != 0) {

                //execute the instruction
                switch(opcode) {
                    //ADD
                    case 1:
                    //ADDI
                    case 2:
                        result = operands[1] + operands[2];
                        break;
                    //AND
                    case 3:
                        result = operands[1] && operands[2];
                        break;
                    //MULT
                    case 4:
                        result = operands[1] * operands[2];
                        break;
                    //OR
                    case 5:
                        result = operands[1] || operands[2];
                        break;
                    //SUB
                    case 6:
                        result = operands[1] - operands[2];
                        break;
                }

                printf("opcode: %d\n", opcode);
                printf("operands: %d %d %d\n", operands[0], operands[1], operands[2]);

                //write the result to the output register
                registerFile->setRegisterValue(operands[0], result);

                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;

                //print the instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(DEBUG_Instruction);

                //reset variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                result = 0;
                DEBUG_Instruction = (Instruction) {0,0,0,0};
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int* x) {
            operands = x;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //reset debug instruciton
            DEBUG_Instruction = (Instruction) {0,0,0,0};
        }
};
