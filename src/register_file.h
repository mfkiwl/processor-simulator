using namespace std;
class RegisterFile {
	int numOfRegisters;
	int* registers;
    int* scoreboard;
    public: 
    	RegisterFile(int numOfRegisters) : numOfRegisters(numOfRegisters) {
    		registers = new int[numOfRegisters];
    		for(int i = 0; i < numOfRegisters; i++) {
    			registers[i] = 0;
    		}
            scoreboard = new int[numOfRegisters];
            for(int i = 0; i < numOfRegisters; i++) {
                scoreboard[i] = 1;
            }
    	}

        int getNumOfRegisters() {
        	return numOfRegisters;
        }

        int getRegisterValue(int i) {
            if(i < 0 || i >= numOfRegisters) {
                cout << "Register index " << i << " is out of range.\n";
            }
            return registers[i];
        }

        void setRegisterValue(int i, int val) {
            if(i < 0 || i >= numOfRegisters) {
                cout << "Register index " << i << " is out of range.\n";
            }
            registers[i] = val;
        }

        int getScoreboardValue(int i ) {
            if(i < 0 || i >= numOfRegisters) {
                cout << "Scoreboard index " << i << " is out of range.\n";
            }
            return scoreboard[i];
        }

        void setScoreboardValue(int i, int val) {
            if(i < 0 || i >= numOfRegisters) {
                cout << "Scoreboard index " << i << " is our of range.\n";
            }
            scoreboard[i] = val;
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
        	cout << endl;

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