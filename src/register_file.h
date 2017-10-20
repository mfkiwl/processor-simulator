#include <stdlib.h>

class RegisterFile {
	int numRegisters;
    public: 
    	RegisterFile(int a) : numRegisters(a) {}

        int getNumRegisters() {
        	return numRegisters;
        }
};