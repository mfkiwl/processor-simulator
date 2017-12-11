class MemoryUnit {
    Memory* memory;
    RegisterFile* registerFile;
    int opcode;
    int* operands;

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
                    //LW
                    case 7:
                        address = 0 + operands[1];
                        value = memory->loadFromMemory(address);
                        registerFile->setRegisterValue(operands[0], value);
                        break;
                    //SW
    		        case 8:
                        address = 0 + operands[1];
                        value = registerFile->getRegisterValue(operands[0]);
                        memory->storeInMemory(address, value);
                        break;
                }

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }

                //print the instruction that has been executed
                printf("Executed instruction: ");
                printInstruction(DEBUG_Instruction);
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
};
