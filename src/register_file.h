using namespace std;
class RegisterFile {
	int numOfRegisters;
	int* registers;
    public: 
    	RegisterFile(int numOfRegisters) : numOfRegisters(numOfRegisters) {
    		registers = new int[numOfRegisters];
    		for(int i = 0; i < numOfRegisters; i++) {
    			registers[i] = 0;
    		}
    	}

        int getNumOfRegisters() {
        	return numOfRegisters;
        }

        int getRegisterValue(int i) {
        	if(i < 0 || i > numOfRegisters - 1) {
        		cout << "Register index " << i << " is out of bounds.\n";
        	}
        	return registers[i];
        }

        void setRegisterValue(int i, int val) {
        	if(i < 0 || i > numOfRegisters - 1) {
        		cout << "Register index " << i << " is out of bounds.\n";
        	}
        	registers[i] = val;
        }

        void printRegisters() {

            //print register names
        	cout << "Registers: ";
        	int lengths[numOfRegisters];
        	for(int i = 0; i < numOfRegisters; i++) {
        		lengths[i] = intLength(registers[i]);
        	}
        	for(int i = 0; i < numOfRegisters; i++) {
        		cout << "R" << i << " ";
        		if(lengths[i] > 2) {
        			for(int j = 2; j < lengths[i]; j++) {
                        cout << " ";
        			}
        		}
        	}
        	cout << "\n";

            //print register values
        	cout << "Values:    ";
        	for(int i = 0; i < numOfRegisters; i++) {
        		if(lengths[i] == 1) {
        		    cout << registers[i] << "  ";
        		}
        		else {
        			cout << registers[i] << " ";
        		}
                if(i >= 10) {
                    cout << " ";
                }
        	}
        	cout << "\n";
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