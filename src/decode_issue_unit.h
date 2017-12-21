#ifndef DECODE_ISSUE_UNIT_H
#define DECODE_ISSUE_UNIT_H

class DecodeIssueUnit {
    //forward components
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;
    ALUReservationStation* aluReservationStation;
    BranchUnitReservationStation* branchUnitReservationStation;
    LoadStoreUnitReservationStation* loadStoreUnitReservationStation;
    LoadStoreUnit* loadStoreUnit;

    //Instruction given my fetch unit
    Instruction nextInstruction;

    //for debugging purposes
    Instruction currentInstruction;

    //Decoding of the instruction
    int opcode;
    int operands[3];

    //tells the processor whether or not to block the pipeline
    int* blockingFlag;

    public:
    	DecodeIssueUnit(RegisterFile* registerFile, ReorderBuffer* reorderBuffer, ALUReservationStation* aluReservationStation, BranchUnitReservationStation* branchUnitReservationStation, LoadStoreUnitReservationStation* loadStoreUnitReservationStation, int* blockingFlag) :
            registerFile(registerFile),
            reorderBuffer(reorderBuffer),
    	    aluReservationStation(aluReservationStation),
    	    branchUnitReservationStation(branchUnitReservationStation),
    	    loadStoreUnitReservationStation(loadStoreUnitReservationStation),
            nextInstruction((Instruction) {0,0,0,0}),
            currentInstruction((Instruction) {0,0,0,0}),
            opcode(0),
            blockingFlag(blockingFlag)
        {
            //initialise operand values to zero
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            opcode = nextInstruction.opcode;
            for(int i = 0; i < 3; i++) {
                operands[i] = nextInstruction.operands[i];
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
                    //if there is space in the reservation then issue the instruction
                    if(aluReservationStation->findFreePosition() != -1) {
                        //Instruction has been issued so add entry to the reorder buffer
                        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, currentInstruction.operands[0], currentInstruction);
                        //send the instruction to the reservation station
                        aluReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                        //unblock the pipeline
                        *blockingFlag = 0;
                    }
                    //otherwise block the pipeline
                    else {
                        *blockingFlag = 1;
                    }
                    break;

                //Load Store unit instructions
                case LW:
                case LWR:
                    if(loadStoreUnitReservationStation->findFreePosition() != -1) {
                        //Instruction has been issued so add entry to the reorder buffer
                        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, currentInstruction.operands[0], currentInstruction);
                        //send the instruction to the reservation station
                        loadStoreUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);

                        *blockingFlag = 0;
                    }
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case SW:
                case SWR:
                    if(loadStoreUnitReservationStation->findFreePosition() != -1) {
                        //Instruction has been issued so add entry to the reorder buffer
                        reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_MEMORY, currentInstruction.operands[1], currentInstruction);
                        //send the instruction to the reservation station
                        loadStoreUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);

                        *blockingFlag = 0;
                    }
                    else {
                        *blockingFlag = 1;
                    }
                    break;


                //Branch unit instructions
                case BEQ:
                case BNE:
                    if(branchUnitReservationStation->findFreePosition() != -1) {
                        //Instruction has been issued so add entry to the reorder buffer
                        reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[2], currentInstruction);
                        //send the instruction to the reservation station
                        branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);

                        *blockingFlag = 0;
                    }
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                case BGEZ:
                case BGTZ:
                case BLEZ:
                case BLTZ:
                    break;
                case J:
                case JR:
                    if(branchUnitReservationStation->findFreePosition() != -1) {
                        //Instruction has been issued so add entry to the reorder buffer
                        reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[0], currentInstruction);
                        //send the instruction to the reservation station
                        branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);

                        *blockingFlag = 0;
                    }
                    else {
                        *blockingFlag = 1;
                    }
                    break;
                    
                //Instruction to finish the program
                case HALT:
                    if(branchUnitReservationStation->findFreePosition() != -1) {
                        //Instruction has been issued so add entry to the reorder buffer
                        reorderBufferIndex = reorderBuffer->addEntry(SYSCALL, currentInstruction.operands[0], currentInstruction);
                        //send the instruction to the reservation station
                        branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);

                        *blockingFlag = 0;
                    }
                    else {
                        *blockingFlag = 1;
                    }
                    break;
            }
            //reset the decoding
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void print() {
            printf("DECODE ISSUE UNIT:");
            Instructions::printInstruction(currentInstruction);
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