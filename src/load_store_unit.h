#ifndef LOAD_STORE_UNIT_H
#define LOAD_STORE_UNIT_H

class LoadStoreUnit {
    //forward components
    Memory* memory;
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
    StoreBuffer storeBuffer;
    LoadBuffer loadBuffer;

    //position in the reorder buffer
    int reorderBufferIndex;

    public:
        LoadStoreUnit(Memory* memory, ReorderBuffer* reorderBuffer, int* blockingFlag) :
            //set initial opcode value to zero
            opcode(0),
            //connected components
            memory(memory),
            reorderBuffer(reorderBuffer),
            blockingFlag(blockingFlag),
            bufferSize(100),
            writeCycles(5),
            readCycles(5),
            storeBuffer(memory, reorderBuffer, bufferSize, writeCycles),
            loadBuffer(memory, reorderBuffer, bufferSize, readCycles),
            reorderBufferIndex(-1)
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
                        loadBuffer.addToBuffer(operands[0], address, reorderBufferIndex);
                        reorderBuffer->executingEntry(reorderBufferIndex);
                        break;
    		        case SW:
                    case SWR:
                        //get the address in memory to update and the value to update it to
                        value = operands[0];
                        address = 0 + operands[1];
                        //and to the write buffer to be written to memory when ready
                        storeBuffer.addToBuffer(address, value, reorderBufferIndex);
                        break;
                }

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
    	    }

            //increment the step numbers for each inflight read and write instruction
            storeBuffer.stepInstructions();
            loadBuffer.stepInstructions();

            //blockIfWaitingForMemoryOperation();
        }

        void writeResult() {
            //perform the read and write instructions when the step number has been met
            storeBuffer.writeIfReady();
            loadBuffer.readIfReady();
        }

        int blockIfWaitingForMemoryOperation() {
            if(storeBuffer.waitingForStore() || loadBuffer.waitingForReadOperation()) {
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
            reorderBufferIndex = -1;
            storeBuffer.flush();
            loadBuffer.flush();
        }

        void setReorderBufferIndex(int i) {
            reorderBufferIndex = i;
        }

        int waitingForStore() {
            return storeBuffer.waitingForStore();
        }
};

#endif