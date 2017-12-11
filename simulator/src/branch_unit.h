class BranchUnit {
	
    int opcode;
    int inputValue1;
    int inputValue2;
    int branchAddress;
    int* pc;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        BranchUnit(int* pc) : 
            pc(pc),
            opcode(0)
        {}

        void execute() {
            if(opcode != 0) {

                //execute the instruction
                switch(opcode) {
                    //BEQ
                    case 9:
                        if(inputValue1 == inputValue2) {
                            *pc = branchAddress;
                        }
                        break;
                    //BNE
                    case 14:
                        printf("%d %d\n", inputValue1, inputValue2);
                        if(inputValue1 != inputValue2) {
                            *pc = branchAddress;
                        }
                        break;
                    //B
                    case 15:
                        //jump to -1 as the pc will be incremented
                        *pc = branchAddress;
                         break;
                }

                //reset variables
                opcode = 0;
                branchAddress = 0;

                //print the instruction that has been executed
                printf("Executed instruction: ");
                printInstruction(DEBUG_Instruction);
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setInputValues(int a, int b) {
            inputValue1 = a;
            inputValue2 = b;
        }

        void setBranchAddress(int x) {
            branchAddress = x;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }
};
