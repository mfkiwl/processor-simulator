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
    	    //copying the operands
    	    int x = numberOfOperands(opcode);
    	    operands = (int*) malloc(sizeof(int) * x);
    	    for(int i = 0; i < x; i++) {
    	    	operands[i] = currentInstruction.operands[i];
    	    }
        }

        int numberOfOperands(int opcode) {
        	switch(opcode) {
        		case 0:
        		    return 3;
        		    break;
        		case 1:
        		    return 3;
        		    break;
        		case 2:
        		    return 3;
        		    break;
        		case 3:
        		    return 1;
        		    break;
        		case 4:
        		    return 3;
        		    break;
        		case 5:
        		    return 3;
        		    break;
        	}
        	return 0;
        }

        int getOpcode() {
    	    return opcode;
        }

        int* getOperands() {
    	    return operands;
        }
};