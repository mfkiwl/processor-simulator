class DecodeUnit {
    //forward components
    ALU* alu;
    RegisterFile* registerFile;
    BranchUnit* branchUnit;
    LoadStoreUnit* loadStoreUnit;

    //Instruction given my fetch unit
    Instruction instructionRegister;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    //tells the processor whether or not to block the pipeline
    int* blockingFlag;

    //Decoding of the instruction
    int opcode;
    int operands[3];

    public:
    	DecodeUnit(RegisterFile* registerFile, ALU* alu, BranchUnit* branchUnit, LoadStoreUnit* loadStoreUnit, int* blockingFlag) :
            registerFile(registerFile),
    	    alu(alu),
    	    branchUnit(branchUnit),
    	    loadStoreUnit(loadStoreUnit),
            instructionRegister((Instruction) {0,0,0}),
            opcode(0),
            blockingFlag(blockingFlag)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            opcode = instructionRegister.opcode;
            int registerNum;
            int val;
            //Replacing registers with with values
            switch(opcode) {
                case ADD:
                case AND:
                case MULT:
                case OR:
                case SUB:
                    //if the source registers are ready then continue
                    if(registerFile->getScoreboardValue(instructionRegister.operands[1]) && registerFile->getScoreboardValue(instructionRegister.operands[2])) {
                        operands[0] = instructionRegister.operands[0];
                        for(int i = 1; i < 3; i++) {
                            registerNum = instructionRegister.operands[i];
                            val = registerFile->getRegisterValue(registerNum);
                            operands[i] = val;
                        }
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case ADDI:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreboardValue(instructionRegister.operands[1])) {
                        operands[0] = instructionRegister.operands[0];
                        registerNum = instructionRegister.operands[1];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[1] = val;
                        operands[2] = instructionRegister.operands[2];
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case LW:
                case SW:
                    operands[0] = instructionRegister.operands[0];
                    operands[1] = instructionRegister.operands[1];
                    break;
                case LWR:
                case SWR:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreboardValue(instructionRegister.operands[1])) {
                        operands[0] = instructionRegister.operands[0];
                        registerNum = instructionRegister.operands[1];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[1] = val;
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case BEQ:
                case BNE:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreboardValue(instructionRegister.operands[0]) && registerFile->getScoreboardValue(instructionRegister.operands[1])) {
                        registerNum = instructionRegister.operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        registerNum = instructionRegister.operands[1];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[1] = val;
                        operands[2] = instructionRegister.operands[2];
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case BGEZ:
                case BGTZ:
                case BLEZ:
                case BLTZ:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreboardValue(instructionRegister.operands[0])) {
                        registerNum = instructionRegister.operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        operands[1] = instructionRegister.operands[1];
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case J:
                    operands[0] = instructionRegister.operands[0];
                    break;
                case JR:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreboardValue(instructionRegister.operands[0])) {
                        registerNum = instructionRegister.operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
            }
            DEBUG_Instruction = instructionRegister;
        }

        void pipe() {
            /*
            printf("Decode unit instruction: ");
            printInstruction(DEBUG_Instruction);
            printf("opcode: %d\n", opcode);
            printf("operands: %d %d %d\n", operands[0], operands[1], operands[2]);
            */
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
                    alu->set_DEBUG_Instruction(DEBUG_Instruction);
                    //Setting the scoreboard values of the destination register to 0
                    registerFile->setScoreboardValue(operands[0],0);
                    break;
                //Load Store unit instructions
                case 7:
                case 8:
                case 9:
                case 10:
                    loadStoreUnit->setOpcode(opcode);
                    loadStoreUnit->setOperands(operands);
                    loadStoreUnit->set_DEBUG_Instruction(DEBUG_Instruction);
                    //Setting the scoreboard values of the destination register to 0
                    registerFile->setScoreboardValue(operands[0],0);
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
                    branchUnit->set_DEBUG_Instruction(DEBUG_Instruction);
                    break;
            }
            //reset the decoding
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
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
