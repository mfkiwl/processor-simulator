#ifndef BRANCH_UNIT_RESERVATION_STATION_H
#define BRANCH_UNIT_RESERVATION_STATION_H

class BranchUnitReservationStation {
    RegisterFile* registerFile;
    BranchUnit* branchUnit;

	int size;
	Instruction* instructions;
	int* reorderBufferIndexes;

	int opcode;
	int* operands;
	int reorderBufferIndex;

public:
	BranchUnitReservationStation(RegisterFile* registerFile, BranchUnit* branchUnit) : 
	registerFile(registerFile),
	branchUnit(branchUnit),
	size(10),
	opcode(0),
	reorderBufferIndex(-1)
	{
		//allocate memory to the buffer
		instructions = new Instruction[size];
		//inialise all instructions to NOOPs
		for(int i = 0; i < size; i++) {
			instructions[i] = (Instruction) {0,0,0,0};
		}
		//allocate memory to the reorder buffer indexes array
		reorderBufferIndexes = new int[size];
		//initialise all reorder buffer indexes to -1
		for(int i = 0; i < size; i++) {
			reorderBufferIndexes[i] = -1;
		}
		//allocate memory for the operands array
		operands = new int[3];
		//initialising operands
		for(int i = 0; i < 3; i++) {
			operands[i] = 0;
		}
	}

    //dispatch an instruction if it can
	void execute() {
		for(int i = 0; i < size; i++) {
			if(instructions[i].opcode != NOOP) {
				if(readyToDispatch(instructions[i])) {
                    dispatch(instructions[i]);
                    reorderBufferIndex = reorderBufferIndexes[i];
                    printf("DISPATCHING INSTRUCTION: ");
                    Instructions::printInstruction(instructions[i]);
                    printf("WITH REORDER BUFFER INDEX: %d\n", reorderBufferIndex);
                    instructions[i] = (Instruction) {0,0,0,0};
                    break;
				}
			}
		}
	}

	void addInstruction(Instruction instruction, int rbi) {
		printf("ADDED INSTRUCTION: ");
		Instructions::printInstruction(instruction);
		instructions[size - 1] = instruction;
		reorderBufferIndexes[size - 1] = rbi;
	}

	int readyToDispatch(Instruction instruction) {
		//check that the source register are ready to use
		switch(instruction.opcode) {
			case BEQ:
            case BNE:
                if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1])) {
                    return 1;
                }
                break;
            case BGEZ:
            case BGTZ:
            case BLEZ:
            case BLTZ:
                if(registerFile->getScoreBoardValue(operands[0])) {
                    return 1;
                }
                break;
            case HALT:
                return 1;
                break;
		}
		return 0;
	}

    //dispatch bound fetch
    void dispatch(Instruction instruction) {
        //getting the opcode and incomplete operands from the instruction
        opcode = instruction.opcode;
        operands = new int[3];
        for(int i = 0; i < 3; i++) {
        	operands[i] = instruction.operands[i];
        }
        //temp variables
        int registerNum;
        int val;
        //fetching the operands for the instruction
        switch(opcode) {
            case BEQ:
            case BNE:
                registerNum = operands[0];
                val = registerFile->getRegisterValue(registerNum);
                operands[0] = val;
                registerNum = operands[1];
                val = registerFile->getRegisterValue(registerNum);
                operands[1] = val;
                break;
            case BGEZ:
            case BGTZ:
            case BLEZ:
            case BLTZ:
                registerNum = operands[0];
                val = registerFile->getRegisterValue(registerNum);
                operands[0] = val;
                break;
        }
    }

    void pipe() {
    	if(opcode != 0) {
    	    //send the decoded instruction to the execution unit
            branchUnit->setOpcode(opcode);
            branchUnit->setOperands(operands);
            //Send the reorder buffer index to the execution unit
            branchUnit->setReorderBufferIndex(reorderBufferIndex);
        
            //reset the outputs
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            reorderBufferIndex = -1;
        }
    }

    void flush() {
    	for(int i = 0; i < size; i++) {
    		instructions[i] = (Instruction) {0,0,0,0};
    		reorderBufferIndexes[i] = -1;
    	}
    	opcode = 0;
    	for(int i = 0; i < 3; i++) {
    		operands[i] = 0;
    	}
    	reorderBufferIndex = -1;
    }
};

#endif