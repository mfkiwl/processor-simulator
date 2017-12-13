using namespace std;
class MemoryUnit {
    //forward components
    Memory* memory;
    RegisterFile* registerFile;

    //decoded instruction
    int opcode;
    int* operands;

    int* blockingFlag;

    //no of instruction executed
    int* noOfInstructionsExecuted;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    //read buffer
    int** readBuffer;
    int readBufferSize;
    int readBufferStart;
    int readBufferEnd;
    int readBufferSteps;

    //write buffer
    int** writeBuffer;
    int writeBufferSize;
    int writeBufferStart;
    int writeBufferEnd;
    int writeBufferSteps;

    public:
        MemoryUnit(Memory* memory, RegisterFile* registerFile, int* noOfInstructionsExecuted, int* blockingFlag) : 
            memory(memory),
            registerFile(registerFile),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            opcode(0),
            writeBufferSize(100),
            writeBufferStart(0),
            writeBufferEnd(0),
            writeBufferSteps(5),
            readBufferSize(100),
            readBufferStart(0),
            readBufferEnd(0),
            readBufferSteps(5),
            blockingFlag(blockingFlag)
        {
            //dynamically allocated a 2d array to the read and write buffer
            writeBuffer = new int*[writeBufferSize];
            readBuffer = new int*[readBufferSize];
            for(int i = 0; i < writeBufferSize; i++) {
                writeBuffer[i] = new int[3];
                readBuffer[i] = new int[3];
            }
            for(int i = 0; i < writeBufferSize; i++) {
                for(int j = 0; j < 3; j++) {
                    writeBuffer[i][j] = 0;
                    readBuffer[i][j] = 0;
                }
            }
        }

        void execute() {
            if(opcode != 0) {

                int address;
                int value;

                //execute the instruction
    	        switch(opcode) {
                    //LW
                    case 7:
                    //LWR
                    case 8:
                        address = 0 + operands[1];
                        value = memory->loadFromMemory(address);
                        registerFile->setRegisterValue(operands[0], value);
                        //set the scoreboard value of the destination register to 1
                        registerFile->setScoreboardValue(operands[0],1);
                        break;
                    //SW
    		        case 9:
                    //SWR
                    case 10:
                        address = 0 + operands[1];
                        value = registerFile->getRegisterValue(operands[0]);
                        addToWriteBuffer(address, value);
                        break;
                }

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }

                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;

                //print the instruction that has been executed
                printf("Executed instruction: ");
                printInstruction(DEBUG_Instruction);
    	    }

            //perform the write buffer operations
            writeBufferStep();
            writeIfReady();

            //if we are waiting for a load or store to complete then block the pipeline
            if(waitingForMemory()) {
                *blockingFlag = 1;
            }
            else {
                *blockingFlag = 0;
            }
            cout << writeBufferStart << " " << writeBufferEnd << "\n";
        }

        int waitingForMemory() {
            if(writeBufferStart != writeBufferEnd) {
                return 1;
            }
            else {
                return 0;
            }
        }

        void addToWriteBuffer(int address, int value) {
            if(writeBufferStart > 0) {
                writeBufferStart -= 1;
                writeBuffer[writeBufferStart][0] = address;
                writeBuffer[writeBufferStart][1] = value;
                writeBuffer[writeBufferStart][2] = 0;
            }
            else if(writeBufferEnd < writeBufferSize - 1) {
                writeBuffer[writeBufferEnd][0] = address;
                writeBuffer[writeBufferEnd][1] = value;
                writeBuffer[writeBufferEnd][2] = 0;
                writeBufferEnd += 1;
            }
        }

        void writeBufferStep() {
            for(int i = writeBufferStart; i < writeBufferEnd; i++) {
                writeBuffer[i][2] += 1;
            }
        }

        void writeIfReady() {
            for(int i = writeBufferStart; i < writeBufferEnd; i++) {
                //for each entry check if it is ready to write
                if(writeBuffer[i][2] == writeBufferSteps) {
                    //write to memory
                    int address = writeBuffer[i][0];
                    int value = writeBuffer[i][1];
                    memory->storeInMemory(address, value);
                    //reset write buffer entry
                    writeBuffer[i][0] = 0;
                    writeBuffer[i][1] = 0;
                    writeBuffer[i][2] = 0;
                    if(i == writeBufferStart) {
                        writeBufferStart += 1;
                    }
                    else if(i == writeBufferEnd - 1) {
                        writeBufferEnd -= 1;
                    }
                }
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int* x) {
            operands = x;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //reset debug instruciton
            DEBUG_Instruction = (Instruction) {0,0,0,0};
        }
};
