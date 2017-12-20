#ifndef _DECODE_UNIT_INCLUDED_
#define _DECODE_UNIT_INCLUDED_

class DecodeUnit {
    //forward components
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;
    ALUReservationStation* aluReservationStation;
    BranchUnit* branchUnit;
    LoadStoreUnit* loadStoreUnit;

    //Instruction given my fetch unit
    Instruction nextInstruction;

    //for debugging purposes
    Instruction currentInstruction;

    //tells the processor whether or not to block the pipeline
    int* blockingFlag;

    //Decoding of the instruction
    int opcode;
    int operands[3];

    public:
    	DecodeUnit(RegisterFile* registerFile, ReorderBuffer* reorderBuffer, ALUReservationStation* aluReservationStation, BranchUnit* branchUnit, LoadStoreUnit* loadStoreUnit, int* blockingFlag) :
            registerFile(registerFile),
            reorderBuffer(reorderBuffer),
    	    aluReservationStation(aluReservationStation),
    	    branchUnit(branchUnit),
    	    loadStoreUnit(loadStoreUnit),
            nextInstruction((Instruction) {0,0,0,0}),
            currentInstruction((Instruction) {0,0,0,0}),
            opcode(0),
            blockingFlag(blockingFlag)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            opcode = nextInstruction.opcode;
            for(int i = 0; i < 3; i++) {
                operands[i] = nextInstruction.operands[i];
            }
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
                    if(registerFile->getScoreBoardValue(operands[1]) && registerFile->getScoreBoardValue(operands[2])) {
                        for(int i = 1; i < 3; i++) {
                            registerNum = operands[i];
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
                    if(registerFile->getScoreBoardValue(operands[1])) {
                        registerNum = operands[1];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[1] = val;
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case LW:
                    break;
                case SW:
                    if(registerFile->getScoreBoardValue(operands[0])) {
                        registerNum = operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        *blockingFlag = 0;
                    }
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case LWR:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreBoardValue(operands[1])) {
                        registerNum = operands[1];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[1] = val;
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case SWR:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1])) {
                        registerNum = operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        registerNum = operands[1];
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
                    if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1])) {
                        registerNum = operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        registerNum = operands[1];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[1] = val;
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
                    if(registerFile->getScoreBoardValue(operands[0])) {
                        registerNum = operands[0];
                        val = registerFile->getRegisterValue(registerNum);
                        operands[0] = val;
                        *blockingFlag = 0;
                    }
                    //If the source registers aren't ready then block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case J:
                    break;
                case JR:
                    //If the source registers are ready then continue
                    if(registerFile->getScoreBoardValue(operands[0])) {
                        registerNum = operands[0];
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
            currentInstruction = nextInstruction;
        }

        void pipe() {
            int reorderBufferIndex = -1;
            switch(opcode) {
                //ALU instructions
                case ADD:
                case ADDI:
                case AND:
                case MULT:
                case OR:
                case SUB:
                    /*
                    //send the decoded instruction to the execution unit
                    alu->setOpcode(opcode);
                    alu->setOperands(operands);
                    //Setting the scoreBoard values of the destination register to 0
                    registerFile->setScoreBoardValue(operands[0],0);
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, operands[0], currentInstruction);
                    alu->setReorderBufferIndex(reorderBufferIndex);
                    */

                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, currentInstruction.operands[0], currentInstruction);
                    //send the instruction to the reservation station
                    aluReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                    //Setting the scoreBoard values of the destination register to 0
                    registerFile->setScoreBoardValue(currentInstruction.operands[0],0);
                    
                    break;
                //Load Store unit instructions
                case LW:
                case LWR:
                    //send the decoded instruction to the execution unit
                    loadStoreUnit->setOpcode(opcode);
                    loadStoreUnit->setOperands(operands);
                    //Setting the scoreBoard values of the destination register to 0
                    registerFile->setScoreBoardValue(operands[0],0);
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, operands[0], currentInstruction);
                    loadStoreUnit->setReorderBufferIndex(reorderBufferIndex);
                    break;
                case SW:
                case SWR:
                    //send the decoded instruction to the execution unit
                    loadStoreUnit->setOpcode(opcode);
                    loadStoreUnit->setOperands(operands);
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_MEMORY, operands[1], currentInstruction);
                    loadStoreUnit->setReorderBufferIndex(reorderBufferIndex);
                    break;
                //Branch unit instructions
                case BEQ:
                case BNE:
                    //send the decoded instruction to the execution unit
                    branchUnit->setOpcode(opcode);
                    branchUnit->setOperands(operands);
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(JUMP, operands[2], currentInstruction);
                    branchUnit->setReorderBufferIndex(reorderBufferIndex);
                    break;
                case BGEZ:
                case BGTZ:
                case BLEZ:
                case BLTZ:
                case J:
                case JR:
                    //send the decoded instruction to the execution unit
                    branchUnit->setOpcode(opcode);
                    branchUnit->setOperands(operands);
                    break;
                case HALT:
                    //send the decoded instruction to the execution unit
                    branchUnit->setOpcode(opcode);
                    branchUnit->setOperands(operands);
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(SYSCALL, operands[0], currentInstruction);
                    branchUnit->setReorderBufferIndex(reorderBufferIndex);
                    break;
            }
            //reset the decoding
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void setNextInstruction(Instruction x) {
        	nextInstruction = x;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            nextInstruction = (Instruction) {0,0,0,0};
            currentInstruction = (Instruction) {0,0,0,0};
        }
};

#endif