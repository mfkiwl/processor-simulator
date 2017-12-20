#ifndef _DECODE_UNIT_INCLUDED_
#define _DECODE_UNIT_INCLUDED_

class DecodeUnit {
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

    //tells the processor whether or not to block the pipeline
    int* blockingFlag;

    //Decoding of the instruction
    int opcode;
    int operands[3];

    public:
    	DecodeUnit(RegisterFile* registerFile, ReorderBuffer* reorderBuffer, ALUReservationStation* aluReservationStation, BranchUnitReservationStation* branchUnitReservationStation, LoadStoreUnitReservationStation* loadStoreUnitReservationStation) :
            registerFile(registerFile),
            reorderBuffer(reorderBuffer),
    	    aluReservationStation(aluReservationStation),
    	    branchUnitReservationStation(branchUnitReservationStation),
    	    loadStoreUnitReservationStation(loadStoreUnitReservationStation),
            nextInstruction((Instruction) {0,0,0,0}),
            currentInstruction((Instruction) {0,0,0,0}),
            opcode(0)
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
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, currentInstruction.operands[0], currentInstruction);
                    //send the instruction to the reservation station
                    aluReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                    break;

                //Load Store unit instructions
                case LW:
                case LWR:
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_REGISTER, currentInstruction.operands[0], currentInstruction);
                    //send the instruction to the reservation station
                    loadStoreUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                    break;
                case SW:
                case SWR:
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(STORE_TO_MEMORY, currentInstruction.operands[1], currentInstruction);
                    //send the instruction to the reservation station
                    loadStoreUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                    break;

                //Branch unit instructions
                case BEQ:
                case BNE:
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[2], currentInstruction);
                    //send the instruction to the reservation station
                    branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                    break;
                case BGEZ:
                case BGTZ:
                case BLEZ:
                case BLTZ:
                    break;
                case J:
                case JR:
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(JUMP, currentInstruction.operands[0], currentInstruction);
                    //send the instruction to the reservation station
                    branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
                    break;
                    
                //Instruction to finish the program
                case HALT:
                    //Instruction has been issued so add entry to the reorder buffer
                    reorderBufferIndex = reorderBuffer->addEntry(SYSCALL, currentInstruction.operands[0], currentInstruction);
                    //send the instruction to the reservation station
                    branchUnitReservationStation->addInstruction(currentInstruction, reorderBufferIndex);
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