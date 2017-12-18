class WriteBuffer {

    Memory* memory;

	//write buffer to hold inflight write operation info
    int** buffer;
    int size;
    int head;
    int tail;
    int steps;

    //index constants
    const int ADDRESS;
    const int VALUE;
    const int STEP;
    const int READY;

    int numberReady;

    //all the inflight instructions
    Instruction* nextInstructions;

    int* noOfInstructionsExecuted;

public:
	WriteBuffer(Memory* memory, int* noOfInstructionsExecuted, int size, int steps) : 
	    memory(memory),
        noOfInstructionsExecuted(noOfInstructionsExecuted),
	    size(size),
	    head(0),
	    tail(0),
	    steps(steps),
        ADDRESS(0),
        VALUE(1),
        STEP(2),
        READY(3),
        numberReady(0)
	{
        //dynamically allocated a 2d array to the read and write buffer
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = new int[4];
        }
        //initialise all elements of the read and write buffer to zero
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < 4; j++) {
                buffer[i][j] = 0;
            }
        }
        //allocate memory to the list of inflight instructions
        nextInstructions = new Instruction[size];
    }

    void addToBuffer(int address, int value, Instruction nextInstruction) {
        //if the start of the buffer is empty then add here
        if(head > 0) {
            head -= 1;
            buffer[head][ADDRESS] = address;
            buffer[head][VALUE] = value;
            buffer[head][STEP] = 0;
            buffer[head][READY] = 0;
            nextInstructions[head] = nextInstruction;
        }
        //otherwise if the end of the buffer is empty then add here
        else if(tail < size - 1) {
            buffer[tail][ADDRESS] = address;
            buffer[tail][VALUE] = value;
            buffer[tail][STEP] = 0;
            buffer[tail][READY] = 0;
            nextInstructions[tail] = nextInstruction;
            tail += 1;
        }
    }

    void stepInstructions() {
        //increment the current step for all inflight instructions in the write buffer
        for(int i = head; i < tail; i++) {
            buffer[i][STEP] += 1;
        }
    }

    void checkIfReady() {
        numberReady = 0;
        //check if each entry in the buffer is ready to write
        for(int i = head; i < tail; i++) {
            if(buffer[i][STEP] >= steps) {
                buffer[i][READY] = 1;
                numberReady++;
            }
        }
    }

    //return 1 if we are waiting for a write operation to complete
    int waitingForWriteOperation() {
        checkIfReady();
        if((tail - head) != numberReady) {
            return 1;
        }
        else {
            return 0;
        }
    }

    void writeIfReady() {
        for(int i = head; i < tail; i++) {
            //for each entry check if it is ready to write
            if(buffer[i][READY]) {
                //write the value to the memory address
                int address = buffer[i][ADDRESS];
                int value = buffer[i][VALUE];
                memory->storeInMemory(address, value);
                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;
                //print the write instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(nextInstructions[i]);
                //reset write buffer entry
                buffer[i][ADDRESS] = 0;
                buffer[i][VALUE] = 0;
                buffer[i][STEP] = 0;
                buffer[i][READY] = 0;
                nextInstructions[i] = (Instruction) {0,0,0,0};
                //update the start and end of the buffer
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