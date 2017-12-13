class DecodeUnit {
    //forward components
    ALU* alu;
    RegisterFile* registerFile;
    BranchUnit* branchUnit;
    MemoryUnit* memoryUnit;

    //Instruction given my fetch unit
    Instruction instructionRegister;

    //Decoding of the instruction
    int opcode;
    int operands[3];

    public:
    	DecodeUnit(RegisterFile* registerFile, ALU* alu, BranchUnit* branchUnit, MemoryUnit* memoryUnit) :
            registerFile(registerFile),
    	    alu(alu),
    	    branchUnit(branchUnit),
    	    memoryUnit(memoryUnit)
        {}

        void execute() {
            printf("Decode unit instruction: ");
            printInstruction(instructionRegister);
            opcode = instructionRegister.opcode;
            int registerNum;
            int val;
            //Replacing registers with with values
            switch(opcode) {
                //ADD
                case 1:
                //AND
                case 3:
                //MULT
                case 4:
                //OR
                case 5:
                //SUB
                case 6:
                    operands[0] = instructionRegister.operands[0];
                    for(int i = 1; i < 3; i++) {
                        registerNum = instructionRegister.operands[i];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[i] = val;
                    }
                    break;
                //ADDI
                case 2:
                    operands[0] = instructionRegister.operands[0];
                    registerNum = instructionRegister.operands[1];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[1] = val;
                    operands[2] = instructionRegister.operands[2];
                    break;
                //LW
                case 7:
                //SW
                case 9:
                    operands[0] = instructionRegister.operands[0];
                    operands[1] = instructionRegister.operands[1];
                    break;
                //LWR
                case 8:
                //SWR
                case 10:
                    operands[0] = instructionRegister.operands[0];
                    registerNum = instructionRegister.operands[1];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[1] = val;
                    break;
                //BEQ
                case 11:
                //BNE
                case 16:
                    registerNum = instructionRegister.operands[0];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[0] = val;
                    registerNum = instructionRegister.operands[1];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[1] = val;
                    operands[2] = instructionRegister.operands[2];
                    break;
                //BGEZ
                case 12:
                //BGTZ
                case 13:
                //BLEZ
                case 14:
                //BLTZ
                case 15:
                    registerNum = instructionRegister.operands[0];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[0] = val;
                    operands[1] = instructionRegister.operands[1];
                    break;
                //J
                case 17:
                    operands[0] = instructionRegister.operands[0];
                    break;
                //JR
                case 18:
                    registerNum = instructionRegister.operands[0];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[0] = val;
                    break;
            }
        }

        void pipe() {
            switch(opcode) {
                //ALU instructions
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                    alu->setOpcode(opcode);
                    alu->setOperands(operands);
                    alu->set_DEBUG_Instruction(instructionRegister);
                    break;
                //Memory unit instructions
                case 7:
                case 8:
                case 9:
                case 10:
                    memoryUnit->setOpcode(opcode);
                    memoryUnit->setOperands(operands);
                    branchUnit->set_DEBUG_Instruction(instructionRegister);
                    break;
                //Branch unit instructions
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                    branchUnit->setOpcode(opcode);
                    branchUnit->setOperands(operands);
                    memoryUnit->set_DEBUG_Instruction(instructionRegister);
                    break;
            }
        }

        void setInstructionRegister(Instruction x) {
        	instructionRegister = x;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            instructionRegister = (Instruction) {0,0,0,0};
        }
};
