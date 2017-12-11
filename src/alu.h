class ALU {

    RegisterFile* registerFile;
    int opcode;
    int* operands;
    int result;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        ALU(RegisterFile* registerFile) : 
            registerFile(registerFile), 
            opcode(0),
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

                //write the result to the output register
                registerFile->setRegisterValue(operands[0], result);

                //reset variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                result = 0;

                //print the instruction that has been executed
                printf("Executed instruction: ");
                printInstruction(DEBUG_Instruction);
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
