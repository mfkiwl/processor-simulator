class MemoryUnit {
	Memory* memory;
	RegisterFile* registerFile;
    int opcode;
    int readWriteRegister;
    int baseAddress;
    int offset;

    //for debugging purposes
    Instruction instructionRegister;

    public:
        MemoryUnit(Memory* memory, RegisterFile* registerFile) : 
            memory(memory),
            registerFile(registerFile),
            opcode(0)
        {}

        void run() {
            if(opcode != 0) {
                int address;
                int value;
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
                opcode = 0;
                printf("Executed instruction: ");
                printInstruction();
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

        void setDebugInstructionRegister(Instruction i) {
            instructionRegister = i;
        }

        void printInstruction() {
            switch(instructionRegister.opcode) {
                case 1:
                    printf("ADD R%d R%d R%d\n", instructionRegister.operands[0], instructionRegister.operands[1], instructionRegister.operands[2]);
                    break;
                case 2:
                    printf("ADDI R%d R%d %d\n", instructionRegister.operands[0], instructionRegister.operands[1], instructionRegister.operands[2]);
                    break;
                case 3:
                    printf("SUB R%d R%d R%d\n", instructionRegister.operands[0], instructionRegister.operands[1], instructionRegister.operands[2]);
                    break;
                case 4:
                    printf("B %d\n", instructionRegister.operands[0]);
                    break;
                case 5:
                    printf("LD R%d R%d %d\n", instructionRegister.operands[0], instructionRegister.operands[1], instructionRegister.operands[2]);
                    break;
                case 6:
                    printf("STR R%d R%d %d\n", instructionRegister.operands[0], instructionRegister.operands[1], instructionRegister.operands[2]);
                    break;
                default:
                    printf("\n");
                    break;
            }
        }
};