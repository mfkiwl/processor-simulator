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
    Instruction nextInstruction;

    int bufferSize;
    int writeCycles;
    int readCycles;

    //Read and write buffers to store load and store instruciton in operation
    WriteBuffer writeBuffer;
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
            nextInstruction((Instruction) {0,0,0,0}),
            bufferSize(100),
            writeCycles(0),
            readCycles(0),
            writeBuffer(memory, noOfInstructionsExecuted, bufferSize, writeCycles),
            readBuffer(memory, registerFile, noOfInstructionsExecuted, bufferSize, readCycles)
        {
            //initially set all operands to zero
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {

                //variables to hold temperary info
                int destinationRegister;
                int address;
                int value;

                //execute the instruction
    	        switch(opcode) {
                    case LW:
                    case LWR:
                        //get the destination register to update the address to read from
                        destinationRegister = operands[0];
                        address = 0 + operands[1];
                        //and to the read buffer to be read from memory when ready
                        readBuffer.addToBuffer(operands[0], address, nextInstruction);
                        break;
    		        case SW:
                    case SWR:
                        //get the address in memory to update and the value to update it to
                        address = 0 + operands[1];
                        value = registerFile->getRegisterValue(operands[0]);
                        //and to the write buffer to be written to memory when ready
                        writeBuffer.addToBuffer(address, value, nextInstruction);
                        break;
                }

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                nextInstruction = (Instruction) {0,0,0,0};
    	    }

            //increment the step numbers for each inflight read and write instruction
            writeBuffer.stepInstructions();
            readBuffer.stepInstructions();

            blockIfWaitingForMemoryOperation();
        }

        void writeback() {
            //perform the read and write instructions when the step number has been met
            writeBuffer.writeIfReady();
            readBuffer.readIfReady();
        }

        int blockIfWaitingForMemoryOperation() {
            if(writeBuffer.waitingForWriteOperation() || readBuffer.waitingForReadOperation()) {
                *blockingFlag = 1;
            }
            else {
                *blockingFlag = 0;
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

        void setNextInstruction(Instruction i) {
            nextInstruction = i;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //reset debug instruciton
            nextInstruction = (Instruction) {0,0,0,0};
        }
};
