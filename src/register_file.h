#include <stdlib.h>

class RegisterFile {
	int numRegisters;
	int* registers;
    public: 
    	RegisterFile(int a) : numRegisters(a) {
    		registers = (int*) malloc(sizeof(int) * numRegisters);
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
        	printf("Register %d given the value %d.\n", i, val);
        }
};