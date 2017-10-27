#include <stdlib.h>
#include <math.h>

class RegisterFile {
	int pc;
	int numRegisters;
	int* registers;
    public: 
    	RegisterFile(int a) : pc(0), numRegisters(a) {
    		registers = (int*) malloc(sizeof(int) * numRegisters);
    		for(int i = 0; i < numRegisters; i++) {
    			registers[i] = 0;
    		}
    	}

    	int getpc() {
    		return pc;
    	}

    	void incpc() {
    		pc++;
    	}

    	void setpc(int x) {
    		pc = x;
    	}

    	void printpc() {
    		printf("PC: %d\n", pc);
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
            int pclength = intLength(pc);
            if(pclength <= 2) {
            	printf("PC ");
            }
        	int lengths[numRegisters];
        	for(int i = 0; i < numRegisters; i++) {
        		lengths[i] = intLength(registers[i]);
        		
        	}
        	for(int i = 0; i < numRegisters; i++) {
        		if(lengths[i] <= 2) {
        		    printf("R%d ", i);
        		}
        	}
        	printf("\n");

        	printf("Values:    ");

            //printf register values
            if(pclength == 1) {
        		printf("%d  ", pc);
            }
        	else if(pclength == 2) {
        		printf("%d  ", pc);
        	}
        	else {
        		printf("%d ", pc);
        	}
        	for(int i = 0; i < numRegisters; i++) {
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