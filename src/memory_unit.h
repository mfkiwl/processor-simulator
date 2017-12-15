class MemoryUnit {
    //forward components
    Memory* memory;
    RegisterFile* registerFile;

    //decoded instruction
    int opcode;
    int operands[3];

    //tells the processor whether or not to block the pipeline
    int *blockingFlag;

    //no of instruction executed
    int* noOfInstructionsExecuted;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    //all the inflight instructions
    Instruction* DEBUG_Write_Instructions;
    Instruction* DEBUG_Read_Instructions;
    int instructionListSize;

    //read buffer to hold inflight read operation info
    int** readBuffer;
    int readBufferSize;
    int readBufferStart;
    int readBufferEnd;
    int readBufferSteps;

    //write buffer to hold inflight write operation info
    int** writeBuffer;
    int writeBufferSize;
    int writeBufferStart;
    int writeBufferEnd;
    int writeBufferSteps;


    public:
        MemoryUnit(Memory* memory, RegisterFile* registerFile, int* noOfInstructionsExecuted, int* blockingFlag) :
            //set initial opcode value to zero
            opcode(0),
            //connected components
            memory(memory),
            registerFile(registerFile),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            blockingFlag(blockingFlag),
            //initialising write buffer variables
            writeBufferSize(100),
            writeBufferStart(0),
            writeBufferEnd(0),
            writeBufferSteps(5),
            //initialising read buffer variables
            readBufferSize(100),
            readBufferStart(0),
            readBufferEnd(0),
            readBufferSteps(5),
            //initialise list to hold all inflight load/store instructions
            instructionListSize(100)
        {
            //initially set all operands to zero
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //dynamically allocated a 2d array to the read and write buffer
            writeBuffer = new int*[writeBufferSize];
            readBuffer = new int*[readBufferSize];
            for(int i = 0; i < writeBufferSize; i++) {
                writeBuffer[i] = new int[3];
                readBuffer[i] = new int[3];
            }
            //initialise all elements of the read and write buffer to zero
            for(int i = 0; i < writeBufferSize; i++) {
                for(int j = 0; j < 3; j++) {
                    writeBuffer[i][j] = 0;
                    readBuffer[i][j] = 0;
                }
            }
            //allocate memory to the list of inflight instructions
            DEBUG_Write_Instructions = new Instruction[instructionListSize];
            DEBUG_Read_Instructions = new Instruction[instructionListSize];
        }

        void execute() {
            if(opcode != 0) {

                //variables to hold temperary info
                int destinationRegister;
                int address;
                int value;

                //execute the instruction
    	        switch(opcode) {
                    //LW
                    case 7:
                    //LWR
                    case 8:
                        //get the destination register to update the address to read from
                        destinationRegister = operands[0];
                        address = 0 + operands[1];
                        //and to the read buffer to be read from memory when ready
                        addToReadBuffer(operands[0], address);
                        break;
                    //SW
    		        case 9:
                    //SWR
                    case 10:
                        //get the address in memory to update and the value to update it to
                        address = 0 + operands[1];
                        value = registerFile->getRegisterValue(operands[0]);
                        //and to the write buffer to be written to memory when ready
                        addToWriteBuffer(address, value);
                        break;
                }

                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                DEBUG_Instruction = (Instruction) {0,0,0,0};
    	    }

            //increment the step numberf for each read and write instruction
            stepBuffers();
        }

        void writeback() {
            //perform the read and write instructions when the step number has been met
            writeIfReady();
            readIfReady();

            //if we are waiting for a write operation to complete then block the pipeline
            if(waitingForWriteOperation()) {
                *blockingFlag = 1;
            }
            else {
                *blockingFlag = 0;
            }
        }

        //return 1 if we are waiting for a write operation to complete
        int waitingForWriteOperation() {
            if(writeBufferStart != writeBufferEnd) {
                return 1;
            }
            else {
                return 0;
            }
        }

        void addToWriteBuffer(int address, int value) {
            //if the start of the buffer is empty then add here
            if(writeBufferStart > 0) {
                writeBufferStart -= 1;
                writeBuffer[writeBufferStart][0] = address;
                writeBuffer[writeBufferStart][1] = value;
                writeBuffer[writeBufferStart][2] = 0;
                DEBUG_Write_Instructions[writeBufferStart] = DEBUG_Instruction;
            }
            //otherwise if the end of the buffer is empty then add here
            else if(writeBufferEnd < writeBufferSize - 1) {
                writeBuffer[writeBufferEnd][0] = address;
                writeBuffer[writeBufferEnd][1] = value;
                writeBuffer[writeBufferEnd][2] = 0;
                DEBUG_Write_Instructions[writeBufferEnd] = DEBUG_Instruction;
                writeBufferEnd += 1;
            }
        }

        void stepBuffers() {
            //increment the current step for all inflight instructions in the write buffer
            for(int i = writeBufferStart; i < writeBufferEnd; i++) {
                writeBuffer[i][2] += 1;
            }
            //increment the current step for all inflight instructions in the read buffer
            for(int i = readBufferStart; i < readBufferEnd; i++) {
                readBuffer[i][2] += 1;
            }
        }

        void writeIfReady() {
            for(int i = writeBufferStart; i < writeBufferEnd; i++) {
                //for each entry check if it is ready to write
                if(writeBuffer[i][2] >= writeBufferSteps) {
                    //write the value to the memory address
                    int address = writeBuffer[i][0];
                    int value = writeBuffer[i][1];
                    memory->storeInMemory(address, value);
                    //print the write instruction that has been executed
                    cout << "Executed instruction: ";
                    printInstruction(DEBUG_Write_Instructions[i]);
                    //reset write buffer entry
                    writeBuffer[i][0] = 0;
                    writeBuffer[i][1] = 0;
                    writeBuffer[i][2] = 0;
                    DEBUG_Write_Instructions[i] = (Instruction) {0,0,0,0};
                    //update the start and end of the buffer
                    if(i == writeBufferStart) {
                        writeBufferStart += 1;
                    }
                    else if(i == writeBufferEnd - 1) {
                        writeBufferEnd -= 1;
                    }
                }
            }
        }

        void addToReadBuffer(int destinationRegister, int address) {
            //if the start of the buffer is empty then add here
            if(readBufferStart > 0) {
                readBufferStart -= 1;
                readBuffer[readBufferStart][0] = destinationRegister;
                readBuffer[readBufferStart][1] = address;
                readBuffer[readBufferStart][2] = 0;
                DEBUG_Read_Instructions[readBufferStart] = DEBUG_Instruction;
            }
            //if the end of the buffer is empty then add here
            else if(readBufferEnd < readBufferSize - 1) {
                readBuffer[readBufferEnd][0] = destinationRegister;
                readBuffer[readBufferEnd][1] = address;
                readBuffer[readBufferEnd][2] = 0;
                DEBUG_Read_Instructions[readBufferEnd] = DEBUG_Instruction;
                readBufferEnd += 1;
            }
        }

        void readIfReady() {
            for(int i = readBufferStart; i < readBufferEnd; i++) {
                //for each entry check if it is ready to write
                if(readBuffer[i][2] >= readBufferSteps) {
                    //read the value from the memory address and store it in the destination register
                    int destinationRegister = readBuffer[i][0];
                    int address = readBuffer[i][1];
                    int value = memory->loadFromMemory(address);
                    registerFile->setRegisterValue(destinationRegister, value);
                    //set the scoreboard value of the destination register to 1
                    registerFile->setScoreboardValue(destinationRegister,1);
                    //print the instruction that has been executed
                    cout << "Executed instruction: ";
                    printInstruction(DEBUG_Read_Instructions[i]);
                    //reset write buffer entry
                    readBuffer[i][0] = 0;
                    readBuffer[i][1] = 0;
                    readBuffer[i][2] = 0;
                    DEBUG_Read_Instructions[i] = (Instruction) {0,0,0,0};
                    //update the start and the end of the buffer
                    if(i == readBufferStart) {
                        readBufferStart += 1;
                    }
                    else if(i == readBufferEnd - 1) {
                        readBufferEnd -= 1;
                    }
                }
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int x[3]) {
            for(int i = 0; i < 3; i++) {
                operands[i] = x[i];
            }
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
