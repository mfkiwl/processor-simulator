class DecodeUnit {
	FetchUnit *fetchUnit;
    int opcode;
    int *operands;

    public:
    	DecodeUnit(FetchUnit *fetchUnit) :
    	    fetchUnit(fetchUnit)
        {}

        void run() {
    	    Instruction currentInstruction = fetchUnit->getNextInstruction();
    	    opcode = currentInstruction.opcode;
    	    operands = currentInstruction.operands;
    	    printf("%p\n", operands);
    	    for(int i = 0; i < 3; i++) {
    	    	printf("%d ", operands[i]);
    	    }
    	    printf("\n");
    	    printf("decode unit has run.\n");
        }

        int getOpcode() {
        	printf("%p\n", operands);
    	    for(int i = 0; i < 3; i++) {
    	    	printf("%d ", operands[i]);
    	    }
    	    printf("\n");
    	    return opcode;
        }

        int* getOperands() {
        	printf("%p\n", operands);
    	    for(int i = 0; i < 3; i++) {
    	    	printf("%d ", operands[i]);
    	    }
    	    printf("\n");
    	    return operands;
        }
};