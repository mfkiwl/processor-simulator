class ALU {

    RegisterFile* registerFile;
    int opcode;
    int outputRegister;
    int inputValue1;
    int inputValue2;
    //the value that we calculate
    int output;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        ALU(RegisterFile* registerFile) : 
            registerFile(registerFile), 
            opcode(0),
            outputRegister(0),
            inputValue1(0),
            inputValue2(0),
            output(0)
        {}

        void run() {
            if(opcode != 0) {
                switch(opcode) {
                    //ADD
                    case 1:
                    //ADDI
                    case 2:
                        output = inputValue1 + inputValue2;
                        break;
                    //SUB
                    case 3:
                        output = inputValue1 - inputValue2;
                        break;
                }
                registerFile->setRegisterValue(outputRegister, output);
                opcode = 0;
                outputRegister = 0;
                inputValue1 = 0;
                inputValue2 = 0;
                output = 0;
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