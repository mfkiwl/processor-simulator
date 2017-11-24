class BranchUnit {
	
    int opcode;
    int branchAddress;
    int* pc;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        BranchUnit(int* pc) : 
            pc(pc),
            opcode(0)
        {}

        void run() {
            if(opcode != 0) {
        	    switch(opcode) {
                    //B
        		    case 4:
        		        *pc = branchAddress;
        		        break;
        	    }
                opcode = 0;
                branchAddress = 0;
                printf("Executed instruction: ");
                printInstruction();
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setBranchAddress(int x) {
            branchAddress = x;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }

        void printInstruction() {
            switch(DEBUG_Instruction.opcode) {
                case 1:
                    printf("ADD R%d R%d R%d\n", DEBUG_Instruction.operands[0], DEBUG_Instruction.operands[1], DEBUG_Instruction.operands[2]);
                    break;
                case 2:
                    printf("ADDI R%d R%d %d\n", DEBUG_Instruction.operands[0], DEBUG_Instruction.operands[1], DEBUG_Instruction.operands[2]);
                    break;
                case 3:
                    printf("SUB R%d R%d R%d\n", DEBUG_Instruction.operands[0], DEBUG_Instruction.operands[1], DEBUG_Instruction.operands[2]);
                    break;
                case 4:
                    printf("B %d\n", DEBUG_Instruction.operands[0]);
                    break;
                case 5:
                    printf("LD R%d R%d %d\n", DEBUG_Instruction.operands[0], DEBUG_Instruction.operands[1], DEBUG_Instruction.operands[2]);
                    break;
                case 6:
                    printf("STR R%d R%d %d\n", DEBUG_Instruction.operands[0], DEBUG_Instruction.operands[1], DEBUG_Instruction.operands[2]);
                    break;
                default:
                    printf("\n");
                    break;
            }
        }
};