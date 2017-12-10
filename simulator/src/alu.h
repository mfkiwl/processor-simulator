class ALU {

    RegisterFile* registerFile;
    int opcode;
    int outputRegister;
    int inputValue1;
    int inputValue2;
    //the value that we calculate
    int result;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        ALU(RegisterFile* registerFile) : 
            registerFile(registerFile), 
            opcode(0),
            outputRegister(0),
            inputValue1(0),
            inputValue2(0),
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
                        result = inputValue1 + inputValue2;
                        break;
                    //AND
                    case 3:
                        result = inputValue1 && inputValue2;
                        break;
                    //MULT
                    case 4:
                        result = inputValue1 * inputValue2;
                        break;
                    //OR
                    case 5:
                        result = inputValue1 || inputValue2;
                        break;
                    //SUB
                    case 6:
                        result = inputValue1 - inputValue2;
                        break;
                }

                //write the result to the output register
                registerFile->setRegisterValue(outputRegister, result);

                //reset variables
                opcode = 0;
                outputRegister = 0;
                inputValue1 = 0;
                inputValue2 = 0;
                result = 0;

                //print the instruction that has been executed
                printf("Executed instruction: ");
                printInstruction(DEBUG_Instruction);
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOutputRegister(int x) {
            outputRegister = x;
        }

        void setInputValues(int a, int b) {
            inputValue1 = a;
            inputValue2 = b;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }
};
