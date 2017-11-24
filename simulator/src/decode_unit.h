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

        void run() {
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
                case 4:
                    operands[0] = instructionRegister.operands[0];
                    break;
                //LD
                case 5:
                    operands[0] = instructionRegister.operands[0];
                    registerNum = instructionRegister.operands[1];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[1] = val;
                    operands[2] = instructionRegister.operands[2];
                    break;
                //STR
                case 6:
                    operands[0] = instructionRegister.operands[0];
                    registerNum = instructionRegister.operands[1];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[1] = val;
                    operands[2] = instructionRegister.operands[2];
                    break;
            }
            //giving the components the instruction that they are executing for debugging purposes
            alu->set_DEBUG_Instruction(instructionRegister);
            branchUnit->set_DEBUG_Instruction(instructionRegister);
            memoryUnit->set_DEBUG_Instruction(instructionRegister);
        }

        void update() {
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
                case 4:
                    branchUnit->setOpcode(opcode);
                    branchUnit->setBranchAddress(operands[0]);
                    break;
                //Memory unit instructions
                case 5:
                case 6:
                    memoryUnit->setOpcode(opcode);
                    memoryUnit->setReadWriteRegister(operands[0]);
                    memoryUnit->setBaseAddress(operands[1]);
                    memoryUnit->setOffset(operands[2]);
                    break;
            }
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

        void setInstructionRegister(Instruction x) {
        	instructionRegister = x;
        }
};