#ifndef _LOAD_STORE_UNIT_INCLUDED_
#define _LOAD_STORE_UNIT_INCLUDED_

class LoadStoreUnit {
    //forward components
    Memory* memory;
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //tells the processor whether or not to block the pipeline
    int *blockingFlag;

    int bufferSize;
    int writeCycles;
    int readCycles;

    //Read and write buffers to store load and store instruciton in operation
    StoreQueue storeQueue;
    LoadQueue loadQueue;

    //position in the reorder buffer
    int reorderBufferIndex;

    public:
        LoadStoreUnit(Memory* memory, RegisterFile* registerFile, ReorderBuffer* reorderBuffer, int* blockingFlag) :
            //set initial opcode value to zero
            opcode(0),
            //connected components
            memory(memory),
            registerFile(registerFile),
            reorderBuffer(reorderBuffer),
            blockingFlag(blockingFlag),
            bufferSize(100),
            writeCycles(5),
            readCycles(5),
            storeQueue(memory, reorderBuffer, bufferSize, writeCycles),
            loadQueue(memory, registerFile, reorderBuffer, bufferSize, readCycles)
        {
            //initially set all operands to zero
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {

                //tell reorder buffer that we are executing the instruction
                reorderBuffer->executingEntry(reorderBufferIndex);

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
                        loadQueue.addToBuffer(operands[0], address, reorderBufferIndex);
                        reorderBuffer->executingEntry(reorderBufferIndex);
                        break;
    		        case SW:
                    case SWR:
                        //get the address in memory to update and the value to update it to
                        address = 0 + operands[1];
                        value = registerFile->getRegisterValue(operands[0]);
                        //and to the write buffer to be written to memory when ready
                        storeQueue.addToBuffer(address, value, reorderBufferIndex);
                        break;
                }

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
    	    }

            //increment the step numbers for each inflight read and write instruction
            storeQueue.stepInstructions();
            loadQueue.stepInstructions();

            blockIfWaitingForMemoryOperation();
        }

        void writeback() {
            //perform the read and write instructions when the step number has been met
            storeQueue.writeIfReady();
            loadQueue.readIfReady();
        }

        int blockIfWaitingForMemoryOperation() {
            if(storeQueue.waitingForWriteOperation() || loadQueue.waitingForReadOperation()) {
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

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void setReorderBufferIndex(int i) {
            reorderBufferIndex = i;
        }

        void setHead(int rbi) {
            storeQueue.setHead(rbi);
        }

        void printStoreQueue() {
            storeQueue.printStoreQueue();
        }
};

#endif