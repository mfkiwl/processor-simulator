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
                printInstruction(DEBUG_Instruction);
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
};