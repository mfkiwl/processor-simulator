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
            writeBuffer(memory, 100, 5),
            readBuffer(memory, registerFile, 100, 5)
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
                        writeBuffer.addToBuffer(address, value, DEBUG_Instruction);
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

            //increment the step numbers for each inflight read and write instruction
            writeBuffer.stepInstructions();
            readBuffer.stepInstructions();
        }

        void writeback() {
            //perform the read and write instructions when the step number has been met
            writeBuffer.writeIfReady();
            readBuffer.readIfReady();

            //if we are waiting for a write operation to complete then block the pipeline
            if(writeBuffer.waitingForWriteOperation()) {
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
