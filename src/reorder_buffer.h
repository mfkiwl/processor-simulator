#ifndef _REORDER_BUFFER_INCLUDED_
#define _REORDER_BUFFER_INCLUDED_

//forward decleared dependencies
class LoadStoreUnit;

class ReorderBuffer {

    RegisterFile* registerFile;
    Memory* memory;
    LoadStoreUnit* loadStoreUnit;

    int* pc;
    int* flushFlag;

    int** buffer;
    int size;
    int head;
    int tail;

    int bufferEntryFields;

    //stores the original instruction issued
    Instruction* instructions;

    //no of instruction executed by the processor
    int* noOfInstructionsExecuted;

    public:
    ReorderBuffer(RegisterFile* registerFile, Memory* memory, LoadStoreUnit* loadStoreUnit, int* pc, int* flushFlag, int size, int* noOfInstructionsExecuted) : 
        registerFile(registerFile),
        memory(memory),
        loadStoreUnit(loadStoreUnit),
        pc(pc),
        flushFlag(flushFlag),
        size(size),
        head(0),
        tail(0),
        bufferEntryFields(4),
        noOfInstructionsExecuted(noOfInstructionsExecuted)
    {
    	//dynamically allocated a 2d array to the read and write buffer
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
            buffer[i] = new int[bufferEntryFields];
        }
        //initialise all elements of the read and write buffer to zero
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < bufferEntryFields; j++) {
                buffer[i][j] = -1;
            }
        }
        //allocate memory to the list of inflight instructions
        instructions = new Instruction[size];
    }

    int addEntry(Type type, int destination, Instruction instruction) {
    	buffer[head][TYPE] = type;
    	buffer[head][DESTINATION] = destination;
    	buffer[head][RESULT] = 0;
    	buffer[head][STATUS] = ISSUED;
        instructions[head] = instruction;
    	int index = head;
    	head = (head + 1) % size;
    	return index;
    }

    void retire() {
    	while(buffer[tail][STATUS] == FINISHED) {
            buffer[tail][STATUS] = -1;
            if(buffer[tail][TYPE] = STORE_TO_REGISTER) {
                printf("STORE TO REGISTER\n");
                //write the result to the reorder buffer
                registerFile->setRegisterValue(buffer[tail][DESTINATION], buffer[tail][RESULT]);
                //Set the scoreBoard of the destination register to 1
                registerFile->setScoreBoardValue(buffer[tail][DESTINATION], 1);
            }
            else if(buffer[tail][TYPE] = JUMP) {
                printf("JUMPING\n");
                *pc = buffer[tail][DESTINATION];
                *flushFlag = 1;
            }
            cout << endl << "Finished Instruction: ";
            printInstruction(instructions[tail]);
            (*noOfInstructionsExecuted)++;
            tail = (tail + 1) % size;
    	}
        //printInstruction(instructions[tail]);
    }

    void executingEntry(int i) {
        buffer[i][STATUS] = EXECUTING;
    }

    void finishedEntry(int i, int result) {
        buffer[i][STATUS] = FINISHED;
        buffer[i][RESULT] = result;
    }

    void writeResult(int i, int r) {
        buffer[i][RESULT] = r;
    }

    void printBuffer() {
    	printf("Reorder buffer: \n");
    	printf("tail: %d\n", tail);
    	printf("head: %d\n", head);
    }
};

#endif