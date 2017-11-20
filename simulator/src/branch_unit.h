class BranchUnit {
	
    RegisterFile *registerFile;
    DecodeUnit *decodeUnit;

    public:
        BranchUnit(RegisterFile *registerFile, DecodeUnit *decodeUnit) : registerFile(registerFile), decodeUnit(decodeUnit) {}

        void run() {
        	int opcode = decodeUnit->getOpcode();
        	int *operands = decodeUnit->getOperands();
        	switch(opcode) {
        		case 3:
        		    B(operands[0]);
        		    break;
        	}
        	printf("branch unit has run.\n");
        }

    private:
        void B(int x) {
        	printf("B %d\n", x);
        	//set to x-1 since the program will automatically be incremented
        	registerFile->setpc(x-1);
        }
};