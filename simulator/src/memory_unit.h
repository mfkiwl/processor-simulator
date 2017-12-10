class MemoryUnit {
    Memory* memory;
    RegisterFile* registerFile;
    int opcode;
    int readWriteRegister;
    int baseAddress;
    int offset;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        MemoryUnit(Memory* memory, RegisterFile* registerFile) : 
            memory(memory),
            registerFile(registerFile),
            opcode(0)
        {}

        void execute() {
            if(opcode != 0) {

                int address;
                int value;

                //execute the instruction
    	        switch(opcode) {
                    //LD
                    case 5:
                        address = baseAddress + offset;
                        value = memory->loadFromMemory(address);
                        registerFile->setRegisterValue(readWriteRegister, value);
                        break;
                    //STR
    		        case 6:
                        address = baseAddress + offset;
                        value = registerFile->getRegisterValue(readWriteRegister);
                        memory->storeInMemory(address, value);
                        break;
                }

                //reset the variables
                opcode = 0;
                readWriteRegister = 0;
                baseAddress = 0;
                offset = 0;

                //print the instruction that has been executed
                printf("Executed instruction: ");
                printInstruction(DEBUG_Instruction);
    	    }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setReadWriteRegister(int x) {
            readWriteRegister = x;
        }

        void setBaseAddress(int x) {
            baseAddress = x;
        }

        void setOffset(int x) {
            offset = x;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }
};
