#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

using namespace std;
class RegisterFile {
	int numOfRegisters;
	int* registers;
    int* scoreBoard;
    public: 
    	RegisterFile(int numOfRegisters) : numOfRegisters(numOfRegisters) {
    		registers = new int[numOfRegisters];
    		for(int i = 0; i < numOfRegisters; i++) {
    			registers[i] = 0;
    		}
            scoreBoard = new int[numOfRegisters];
            for(int i = 0; i < numOfRegisters; i++) {
                scoreBoard[i] = 1;
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

        int getScoreBoardValue(int i ) {
            if(i < 0 || i >= numOfRegisters) {
                cout << "ScoreBoard index " << i << " is out of range.\n";
            }
            return scoreBoard[i];
        }

        void setScoreBoardValue(int i, int val) {
            if(i < 0 || i >= numOfRegisters) {
                cout << "ScoreBoard index " << i << " is our of range.\n";
            }
            scoreBoard[i] = val;
        }

        void resetScoreBoard() {
            for(int i = 0; i < numOfRegisters; i++) {
                scoreBoard[i] = 1;
            }
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
            cout << endl;
        }

        void printScoreBoard() {
            printf("ScoreBoard:\n");
            for(int i = 0; i < numOfRegisters; i++) {
                cout << scoreBoard[i] << " ";
            }
            cout << endl;
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

#endif