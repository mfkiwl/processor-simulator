class RegisterFile {
	int numOfRegisters;
	int* registers;
    public: 
    	RegisterFile(int numOfRegisters) : numOfRegisters(numOfRegisters) {
    		registers = (int*) malloc(sizeof(int) * numOfRegisters);
    		for(int i = 0; i < numOfRegisters; i++) {
    			registers[i] = 0;
    		}
    	}

        int getNumOfRegisters() {
        	return numOfRegisters;
        }

        int getRegisterValue(int i) {
        	if(i < 0 || i > numOfRegisters - 1) {
        		printf("Register index %d is out of bounds.\n", i);
        	}
        	return registers[i];
        }

        void setRegisterValue(int i, int val) {
        	if(i < 0 || i > numOfRegisters - 1) {
        		printf("Register index %d is out of bounds.\n", i);
        	}
        	registers[i] = val;
        }

        void printRegisters() {

            //print register names
        	printf("Registers: ");
        	int lengths[numOfRegisters];
        	for(int i = 0; i < numOfRegisters; i++) {
        		lengths[i] = intLength(registers[i]);
        	}
        	for(int i = 0; i < numOfRegisters; i++) {
        		printf("R%d ", i);
        		if(lengths[i] > 2) {
        			for(int j = 2; j < lengths[i]; j++) {
                        printf(" ");
        			}
        		}
        	}
        	printf("\n");

            //print register values
        	printf("Values:    ");
        	for(int i = 0; i < numOfRegisters; i++) {
        		if(lengths[i] == 1) {
        		    printf("%d  ", registers[i]);
        		}
        		else {
        			printf("%d ", registers[i]);
        		}
                if(i >= 10) {
                    printf(" ");
                }
        	}
        	printf("\n");
        }

        int intLength(int n) {
        	if(n == 0) {
        	    return 1;
        	} else {
        	    int nDigits = floor(log10(abs(n))) + 1;
        	    if(n < 0) {
        	    	nDigits++;
        	    }
        	    return nDigits;
        	}
        }
};