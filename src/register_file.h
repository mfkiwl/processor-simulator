#include <stdlib.h>

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

        void printRegisterValues() {
        	for(int i = 0; i < numRegisters; i++) {
        		printf("%d  ", registers[i]);
        	}
        	printf("\n");
        }
};