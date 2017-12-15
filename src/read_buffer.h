class ReadBuffer {

    RegisterFile* registerFile;
    Memory* memory;

    int** buffer;
    int size;
    int head;
    int tail;
    int steps;

    Instruction* DEBUG_Instructions;

public:
	ReadBuffer(RegisterFile* registerFile, Memory* memory, int size, int steps) : 
	    registerFile(registerFile),
	    memory(memory),
	    size(size), 
	    steps(steps)
	{
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = new int[3];
        }
        //initialise all elements of the read and write buffer to zero
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < 3; j++) {
                buffer[i][j] = 0;
            }
        }
        //allocate memory to the list of inflight instructions
        DEBUG_Instructions = new Instruction[size];
	}

	void addToBuffer(int destinationRegister, int address, Instruction DEBUG_Instruction) {
        //if the start of the buffer is empty then add here
        if(head > 0) {
            head -= 1;
            buffer[head][0] = destinationRegister;
            buffer[head][1] = address;
            buffer[head][2] = 0;
            DEBUG_Instructions[head] = DEBUG_Instruction;
        }
        //if the end of the buffer is empty then add here
        else if(tail < size - 1) {
            buffer[tail][0] = destinationRegister;
            buffer[tail][1] = address;
            buffer[tail][2] = 0;
            DEBUG_Instructions[tail] = DEBUG_Instruction;
            tail += 1;
        }
    }

    void readIfReady() {
        for(int i = head; i < tail; i++) {
            //for each entry check if it is ready to write
            if(buffer[i][2] >= steps) {
                //read the value from the memory address and store it in the destination register
                int destinationRegister = buffer[i][0];
                int address = buffer[i][1];
                int value = memory->loadFromMemory(address);
                registerFile->setRegisterValue(destinationRegister, value);
                //set the scoreboard value of the destination register to 1
                registerFile->setScoreboardValue(destinationRegister,1);
                //print the instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(DEBUG_Instructions[i]);
                //reset write buffer entry
                buffer[i][0] = 0;
                buffer[i][1] = 0;
                buffer[i][2] = 0;
                DEBUG_Instructions[i] = (Instruction) {0,0,0,0};
                //update the start and the end of the buffer
                if(i == head) {
                    head += 1;
                }
                else if(i == tail - 1) {
                    tail -= 1;
                }
            }
        }
    }

    void stepInstructions() {
    	//increment the current step for all inflight instructions in the read buffer
        for(int i = head; i < tail; i++) {
            buffer[i][2] += 1;
        }
    }
};