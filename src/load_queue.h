class LoadQueue {
	Memory* memory;
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;

    //write buffer to hold inflight write operation info
    int** buffer;
    int size;
    int head;
    int tail;
    int steps;

    int queueFields;

    //index constants
    const int DESTINATION;
    const int ADDRESS;
    const int REORDER_BUFFER_INDEX;
    const int STEP;

    //the number of entries in the buffer ready to read
    int numberReady;

public:
	LoadQueue(Memory* memory, RegisterFile* registerFile, ReorderBuffer* reorderBuffer, int size, int steps) : 
        memory(memory),
        registerFile(registerFile),
        reorderBuffer(reorderBuffer),
        size(size),
        head(0),
        tail(0),
        steps(steps),
        queueFields(4),
        DESTINATION(0),
        ADDRESS(1),
        REORDER_BUFFER_INDEX(2),
        STEP(3),
        numberReady(0)
    {
        //dynamically allocated a 2d array to the read and write buffer
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = new int[queueFields];
        }
        //initialise all elements of the read and write buffer to zero
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < queueFields; j++) {
                buffer[i][j] = 0;
            }
        }
    }

    void stepInstructions() {
        //increment the current step for all inflight instructions in the read buffer
        for(int i = head; i < tail; i++) {
            buffer[i][STEP] += 1;
        }
    }

    void checkIfReady() {
        numberReady = 0;
        //check if each entry in the buffer is ready to write
        for(int i = head; i < tail; i++) {
            if(buffer[i][STEP] >= steps) {
                numberReady++;
            }
        }
    }

    //return 1 if we are waiting for a write operation to complete
    int waitingForReadOperation() {
        checkIfReady();
        if(head != tail) {
            return 1;
        }
        else {
            return 0;
        }
    }

    void addToBuffer(int destinationRegister, int address, int reorderBufferIndex) {
        //if the start of the buffer is empty then add here
        if(head > 0) {
            head -= 1;
            buffer[head][DESTINATION] = destinationRegister;
            buffer[head][ADDRESS] = address;
            buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
            buffer[head][STEP] = 0;
        }
        //if the end of the buffer is empty then add here
        else if(tail < size - 1) {
            buffer[tail][DESTINATION] = destinationRegister;
            buffer[tail][ADDRESS] = address;
            buffer[tail][REORDER_BUFFER_INDEX] = reorderBufferIndex;
            buffer[tail][STEP] = 0;
            tail += 1;
        }
    }

    void readIfReady() {
        for(int i = head; i < tail; i++) {
            //for each entry check if it is ready to write
            if(buffer[i][STEP] > steps) {
                //read the value from the memory address and store it in the destination register
                int destinationRegister = buffer[i][DESTINATION];
                int address = buffer[i][ADDRESS];
                int value = memory->loadFromMemory(address);
                registerFile->setRegisterValue(destinationRegister, value);
                //tell the reorder buffer that we are finished executing the instruction
                reorderBuffer->finishedEntry(buffer[i][REORDER_BUFFER_INDEX], 0);
                //set the scoreBoard value of the destination register to 1
                registerFile->setScoreBoardValue(destinationRegister,1);
                //reset write buffer entry
                buffer[i][DESTINATION] = 0;
                buffer[i][ADDRESS] = 0;
                buffer[i][STEP] = 0;
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