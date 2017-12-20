#ifndef STORE_BUFFER_H
#define STORE_BUFFER_H

class StoreBuffer {

    Memory* memory;
    ReorderBuffer* reorderBuffer;

	//write buffer to hold inflight write operation info
    int** buffer;
    int size;
    int head;
    int tail;
    int steps;

    int entryFields;

    //index constants
    const int ADDRESS;
    const int VALUE;
    const int REORDER_BUFFER_INDEX;
    const int STEP;
    const int HEAD;

    int numberReady;

public:
	StoreBuffer(Memory* memory, ReorderBuffer* reorderBuffer, int size, int steps) : 
        reorderBuffer(reorderBuffer),
	    memory(memory),
	    size(size),
	    head(0),
	    tail(0),
        entryFields(5),
	    steps(steps),
        ADDRESS(0),
        VALUE(1),
        REORDER_BUFFER_INDEX(2),
        STEP(3),
        HEAD(4),
        numberReady(0)
	{
        //dynamically allocated a 2d array to the read and write buffer
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = new int[entryFields];
        }
        //initialise all elements of the read and write buffer to zero
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < entryFields; j++) {
                buffer[i][j] = 0;
            }
        }
    }

    void flush() {
        for(int i = 0; i < size; i++) {
            buffer[i][ADDRESS] = 0;
            buffer[i][VALUE] = 0;
            buffer[i][REORDER_BUFFER_INDEX] = 0;
            buffer[i][STEP] = 0;
            buffer[i][HEAD] = 0;
        }
        head = 0;
        tail = 0;
    }

    void addToBuffer(int address, int value, int reorderBufferIndex) {
        //if the start of the buffer is empty then add here
        if(head > 0) {
            head -= 1;
            buffer[head][ADDRESS] = address;
            buffer[head][VALUE] = value;
            buffer[head][REORDER_BUFFER_INDEX] = reorderBufferIndex;
            buffer[head][STEP] = 0;
            buffer[head][HEAD] = 0;
        }
        //otherwise if the end of the buffer is empty then add here
        else if(tail < size - 1) {
            buffer[tail][ADDRESS] = address;
            buffer[tail][VALUE] = value;
            buffer[tail][REORDER_BUFFER_INDEX] = reorderBufferIndex;
            buffer[tail][STEP] = 0;
            buffer[tail][HEAD] = 0;
            tail += 1;
        }
    }

    void printStoreBuffer() {
        for(int i = head; i < tail; i++) {
            printf("HEAD: %d\t", buffer[i][HEAD]);
            printf("STEPS: %d\n", buffer[i][STEP]);
        }
    }

    void stepInstructions() {
        //increment the current step for all inflight instructions in the write buffer
        for(int i = head; i < tail; i++) {
            if(buffer[i][HEAD]) {
                buffer[i][STEP] += 1;
            }
        }
    }

    void setHead(int rbi) {
        for(int i = head; i < tail; i++) {
            if(buffer[i][REORDER_BUFFER_INDEX] == rbi) {
                buffer[i][HEAD] = 1;
                reorderBuffer->executingEntry(buffer[i][REORDER_BUFFER_INDEX]);
                reorderBuffer->setEntryResult(i, 12);
                break;
            }
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
            if(buffer[i][STEP] >= steps) {
                //write the value to the memory address
                int address = buffer[i][ADDRESS];
                int value = buffer[i][VALUE];
                memory->storeInMemory(address, value);
                //printf("STORED %d at ADDRESS %d\n", value, address);
                //tell the reorder buffer that we are finished executing the instruction
                reorderBuffer->finishedEntry(buffer[i][REORDER_BUFFER_INDEX], 0);
                //reset write buffer entry
                buffer[i][ADDRESS] = 0;
                buffer[i][VALUE] = 0;
                buffer[i][REORDER_BUFFER_INDEX] = 0;
                buffer[i][STEP] = 0;
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

#endif