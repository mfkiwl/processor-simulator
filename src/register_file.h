#include <stdlib.h>
#include <math.h>

class RegisterFile {
	int numRegisters;
	int* registers;
    public: 
    	RegisterFile(int a) : numRegisters(a) {
    		registers = (int*) malloc(sizeof(int) * numRegisters);
    		for(int i = 0; i < numRegisters; i++) {
    			registers[i] = 0;
    		}
    	}

    	int getpc() {
    		return registers[0];
    	}

    	void incpc() {
    		registers[0]++;
    	}

    	void setpc(int x) {
    		registers[0] = x;
    	}

    	void printpc() {
    		printf("PC: %d\n", registers[0]);
    	}

        int getNumRegisters() {
        	return numRegisters;
        }

        int getRegisterValue(int i) {
        	if(i < 0 || i > numRegisters - 1) {
        		printf("Register index %d is out of bounds.\n", i);
        	}
        	return registers[i];
        }

        void setRegisterValue(int i, int val) {
        	if(i < 0 || i > numRegisters - 1) {
        		printf("Register index %d is out of bounds.\n", i);
        	}
        	registers[i] = val;
        }

        void printRegisters() {

        	printf("Registers: ");

            //print register names
            int pclength = intLength(registers[0]);
            if(pclength <= 2) {
            	printf("PC ");
            }
        	int lengths[numRegisters];
        	for(int i = 1; i < numRegisters; i++) {
        		lengths[i] = intLength(registers[i]);
        		
        	}
        	for(int i = 1; i < numRegisters; i++) {
        		if(lengths[i] <= 2) {
        		    printf("R%d ", i);
        		}
        	}
        	printf("\n");

        	printf("Values:    ");

            //printf register values
            if(pclength == 1) {
        		printf("%d  ", registers[0]);
            }
        	else if(pclength == 2) {
        		printf("%d  ", registers[0]);
        	}
        	else {
        		printf("%d ", registers[0]);
        	}
        	for(int i = 1; i < numRegisters; i++) {
        		if(lengths[i] == 1) {
        		    printf("%d  ", registers[i]);
        		}
        		else if(lengths[i] == 2) {
        			printf("%d  ", registers[i]);
        		}
        		else {
        			printf("%d ", registers[i]);
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