class MemoryUnit {
    //forward components
    Memory* memory;
    RegisterFile* registerFile;

    //decoded instruction
    int opcode;
    int operands[3];

    //no of instruction executed
    int* noOfInstructionsExecuted;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        MemoryUnit(Memory* memory, RegisterFile* registerFile, int* noOfInstructionsExecuted) : 
            memory(memory),
            registerFile(registerFile),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            opcode(0)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {
/*
                printf("Memory unit:\n");
                printf("opcode: %d\n", opcode);
                printf("operands: %d %d %d\n", operands[0], operands[1], operands[2]);
*/
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
                        memory->storeInMemory(address, value);
                        break;
                }

                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;
/*
                printf("Memory unit:\n");
                printf("opcode: %d\n", opcode);
                printf("operands: %d %d %d\n", operands[0], operands[1], operands[2]);
*/
                //print the instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(DEBUG_Instruction);

                //reset the variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                DEBUG_Instruction = (Instruction) {0,0,0,0};
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
