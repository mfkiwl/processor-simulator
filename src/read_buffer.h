class ReadBuffer {
	Memory* memory;
    RegisterFile* registerFile;

    //write buffer to hold inflight write operation info
    int** buffer;
    int size;
    int head;
    int tail;
    int steps;

    //all the inflight instructions
    Instruction* DEBUG_Instructions;

    int* noOfInstructionsExecuted;

public:
	ReadBuffer(Memory* memory, RegisterFile* registerFile, int* noOfInstructionsExecuted, int size, int steps) : 
        memory(memory),
        registerFile(registerFile),
        noOfInstructionsExecuted(noOfInstructionsExecuted),
        size(size),
        head(0),
        tail(0),
        steps(steps) 
    {
        //dynamically allocated a 2d array to the read and write buffer
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

    void stepInstructions() {
        //increment the current step for all inflight instructions in the read buffer
        for(int i = head; i < tail; i++) {
            buffer[i][2] += 1;
        }
    }

    //return 1 if we are waiting for a write operation to complete
    int waitingForReadOperation() {
        if(head != tail) {
            return 1;
        }
        else {
            return 0;
        }
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
                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;
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
};