class BranchUnit {
	
    int opcode;
    int* operands;
    int* pc;

    public:
        BranchUnit(int* pc) : 
            pc(pc),
            opcode(0),
            operands(NULL)
        {}

        void run() {
            if(opcode != 0 && operands != NULL) {
        	    switch(opcode) {
        		    case 4:
        		        B(operands[0]);
        		        break;
        	    }
                opcode = 0;
                operands = NULL;
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int* x) {
            operands = x;
        }

    private:
        void B(int x) {
        	printf("B %d\n", x);
        	*pc = x;
        }
};