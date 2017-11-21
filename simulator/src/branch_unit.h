class BranchUnit {
	
    RegisterFile *registerFile;
    int opcode;
    int *operands;

    public:
        BranchUnit(RegisterFile *registerFile) : 
            registerFile(registerFile),
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

        void setOperands(int *x) {
            operands = x;
        }

    private:
        void B(int x) {
        	printf("B %d\n", x);
        	//set to x-1 since the program will automatically be incremented
        	registerFile->setpc(x-1);
        }
};