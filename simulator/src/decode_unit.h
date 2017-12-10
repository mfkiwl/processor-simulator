class DecodeUnit {
    ALU* alu;
    RegisterFile* registerFile;
    BranchUnit* branchUnit;
    MemoryUnit* memoryUnit;
    Instruction instructionRegister;
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
            opcode = instructionRegister.opcode;
            int registerNum;
            int val;
            switch(opcode) {
                //ADD
                case 1:
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
                //SUB
                case 3:
                    operands[0] = instructionRegister.operands[0];
                    for(int i = 1; i < 3; i++) {
                        registerNum = instructionRegister.operands[i];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[i] = val;
                    }
                    break;
                //B
                case 15:
                    operands[0] = instructionRegister.operands[0];
                    break;
                //LD
                case 7:
                    operands[0] = instructionRegister.operands[0];
                    operands[1] = instructionRegister.operands[1];
                    break;
                //STR
                case 8:
                    operands[0] = instructionRegister.operands[0];
                    operands[1] = instructionRegister.operands[1];
                    break;
            }
            //giving the components the instruction that they are executing for debugging purposes
            alu->set_DEBUG_Instruction(instructionRegister);
            branchUnit->set_DEBUG_Instruction(instructionRegister);
            memoryUnit->set_DEBUG_Instruction(instructionRegister);
        }

        void pipe() {
            switch(opcode) {
                //ALU instructions
                case 1:
                case 2:
                case 3:
                    alu->setOpcode(opcode);
                    alu->setOutputRegister(operands[0]);
                    alu->setInputValues(operands[1],operands[2]);
                    break;
                //Branch unit instructions
                case 15:
                    branchUnit->setOpcode(opcode);
                    branchUnit->setBranchAddress(operands[0]);
                    break;
                //Memory unit instructions
                case 7:
                case 8:
                    memoryUnit->setOpcode(opcode);
                    memoryUnit->setReadWriteRegister(operands[0]);
                    memoryUnit->setOffset(operands[1]);
                    break;
            }
        }

        void setInstructionRegister(Instruction x) {
        	instructionRegister = x;
        }
};
