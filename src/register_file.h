#include <stdlib.h>

class RegisterFile {
	int numRegisters;
    public: 
    	RegisterFile() {
            numRegisters = 0;
        }
        int getNumRegisters() {
        	return numRegisters;
        }
};