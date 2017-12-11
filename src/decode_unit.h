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
                case 8:
                    operands[0] = instructionRegister.operands[0];
                    operands[1] = instructionRegister.operands[1];
                    break;
                //BEQ
                case 9:
                //BNE
                case 14:
                    registerNum = instructionRegister.operands[0];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[0] = val;
                    registerNum = instructionRegister.operands[1];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[1] = val;
                    operands[2] = instructionRegister.operands[2];
                //BGEZ
                case 10:
                //BGTZ
                case 11:
                //BLEZ
                case 12:
                //BLTZ
                case 13:
                    registerNum = instructionRegister.operands[0];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[0] = val;
                    operands[1] = instructionRegister.operands[1];
                    break;
                //J
                case 15:
                    operands[0] = instructionRegister.operands[0];
                    break;
                //JR
                case 16:
                    registerNum = instructionRegister.operands[0];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[0] = val;
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
                case 4:
                case 5:
                case 6:
                    alu->setOpcode(opcode);
                    alu->setOperands(operands);
                    break;
                //Memory unit instructions
                case 7:
                case 8:
                    memoryUnit->setOpcode(opcode);
                    memoryUnit->setOperands(operands);
                    break;
                //Branch unit instructions
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                    branchUnit->setOpcode(opcode);
                    branchUnit->setOperands(operands);
                    break;
            }
        }

        void setInstructionRegister(Instruction x) {
        	instructionRegister = x;
        }
};
