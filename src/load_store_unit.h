class LoadStoreUnit {
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

    //write buffer to hold inflight write operation info
    int** writeBuffer;
    int writeBufferSize;
    int writeBufferHead;
    int writeBufferTail;
    int writeBufferSteps;

    ReadBuffer readBuffer;


    public:
        LoadStoreUnit(Memory* memory, RegisterFile* registerFile, int* noOfInstructionsExecuted, int* blockingFlag) :
            //set initial opcode value to zero
            opcode(0),
            //connected components
            memory(memory),
            registerFile(registerFile),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            blockingFlag(blockingFlag),
            //initialising write buffer variables
            writeBufferSize(100),
            writeBufferHead(0),
            writeBufferTail(0),
            writeBufferSteps(5),
            //initialise list to hold all inflight load/store instructions
            instructionListSize(100),
            readBuffer(registerFile, memory, 100, 5)
        {
            //initially set all operands to zero
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //dynamically allocated a 2d array to the read and write buffer
            writeBuffer = new int*[writeBufferSize];
            for(int i = 0; i < writeBufferSize; i++) {
                writeBuffer[i] = new int[3];
            }
            //initialise all elements of the read and write buffer to zero
            for(int i = 0; i < writeBufferSize; i++) {
                for(int j = 0; j < 3; j++) {
                    writeBuffer[i][j] = 0;
                }
            }
            //allocate memory to the list of inflight instructions
            DEBUG_Write_Instructions = new Instruction[instructionListSize];
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
                        readBuffer.addToBuffer(operands[0], address, DEBUG_Instruction);
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
            readBuffer.readIfReady();

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
            if(writeBufferHead != writeBufferTail) {
                return 1;
            }
            else {
                return 0;
            }
        }

        void addToWriteBuffer(int address, int value) {
            //if the start of the buffer is empty then add here
            if(writeBufferHead > 0) {
                writeBufferHead -= 1;
                writeBuffer[writeBufferHead][0] = address;
                writeBuffer[writeBufferHead][1] = value;
                writeBuffer[writeBufferHead][2] = 0;
                DEBUG_Write_Instructions[writeBufferHead] = DEBUG_Instruction;
            }
            //otherwise if the end of the buffer is empty then add here
            else if(writeBufferTail < writeBufferSize - 1) {
                writeBuffer[writeBufferTail][0] = address;
                writeBuffer[writeBufferTail][1] = value;
                writeBuffer[writeBufferTail][2] = 0;
                DEBUG_Write_Instructions[writeBufferTail] = DEBUG_Instruction;
                writeBufferTail += 1;
            }
        }

        void stepBuffers() {
            //increment the current step for all inflight instructions in the write buffer
            for(int i = writeBufferHead; i < writeBufferTail; i++) {
                writeBuffer[i][2] += 1;
            }
            readBuffer.stepInstructions();
        }

        void writeIfReady() {
            for(int i = writeBufferHead; i < writeBufferTail; i++) {
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
                    if(i == writeBufferHead) {
                        writeBufferHead += 1;
                    }
                    else if(i == writeBufferTail - 1) {
                        writeBufferTail -= 1;
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
