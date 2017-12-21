#ifndef REORDER_BUFFER_H
#define REORDER_BUFFER_H

class ReorderBuffer {

    RegisterFile* registerFile;
    Memory* memory;

    int* pc;
    int* flushFlag;
    int* runningFlag;

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
    ReorderBuffer(RegisterFile* registerFile, Memory* memory, int* pc, int* flushFlag, int* runningFlag, int* noOfInstructionsExecuted) : 
        registerFile(registerFile),
        memory(memory),
        pc(pc),
        flushFlag(flushFlag),
        runningFlag(runningFlag),
        size(100),
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
        //initialise all of the instructions
        for(int i = 0; i < size; i++) {
            instructions[i] = (Instruction) {0,0,0,0};
        }
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

    int getTailIndex() {
        return tail;
    }

    void retire() {
    	while(buffer[tail][STATUS] == FINISHED) {
            buffer[tail][STATUS] = -1;
            if(buffer[tail][TYPE] == STORE_TO_REGISTER) {
                //write the result to the reorder buffer
                registerFile->setRegisterValue(buffer[tail][DESTINATION], buffer[tail][RESULT]);
                //Set the scoreBoard of the destination register to 1
                registerFile->setScoreBoardValue(buffer[tail][DESTINATION], 1);
            }
            if(buffer[tail][TYPE] == STORE_TO_MEMORY) {

            }
            if(buffer[tail][TYPE] == JUMP && buffer[tail][RESULT]) {
                *pc = buffer[tail][DESTINATION];
                *flushFlag = 1;
            }
            if(buffer[tail][TYPE] == SYSCALL) {
                *runningFlag = 0;
            }
            //print the instruction that we are retiring
            cout << endl << "Retiring Instruction: ";
            Instructions::printInstruction(instructions[tail]);
            //reset the reorder buffer entry
            resetEntry(tail);
            //increment the number of instructions that we have executed
            (*noOfInstructionsExecuted)++;
            //increment the tail position
            tail = (tail + 1) % size;
    	}
    }

    void resetEntry(int index) {
        instructions[index] = (Instruction) {0,0,0,0};
        for(int i = 0; i < bufferEntryFields; i++) {
            buffer[index][i] = -1;
        }
    }

    void executingEntry(int i) {
        buffer[i][STATUS] = EXECUTING;
    }

    void finishedEntry(int i, int result) {
        buffer[i][STATUS] = FINISHED;
        buffer[i][RESULT] = result;
    }

    void setEntryResult(int i, int r) {
        buffer[i][RESULT] = r;
    }

    void writeResult(int i, int r) {
        buffer[i][RESULT] = r;
    }

    void flush() {
        for(int i = 0; i < size; i++) {
            buffer[i][TYPE] = 0;
            buffer[i][DESTINATION] = 0;
            buffer[i][RESULT] = 0;
            buffer[i][STATUS] = 0;
        }
        head = 0;
        tail = 0;
    }

    void printTail() {
        printf("REORDER BUFFER TAIL:");
        Instructions::printInstruction(instructions[tail]);
    }

    void print() {
        cout << "Reorder Buffer:" << endl;
    	for(int i = tail; i < head; i++) {
            Instructions::printInstruction(instructions[i]);
        }
    }
};

#endif