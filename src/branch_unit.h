class BranchUnit {
	
    int opcode;
    int* operands;
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
                    case 11:
                        if(operands[0] == operands[1]) {
                            *pc = operands[2];
                        }
                        break;
                    //BNE
                    case 16:
                        if(operands[0] != operands[1]) {
                            *pc = operands[2];
                        }
                        break;
                    //J
                    case 17:
                        *pc = operands[0];
                         break;
                }

                //reset variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }

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
};
