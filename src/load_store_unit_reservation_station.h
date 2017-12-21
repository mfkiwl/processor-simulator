#ifndef LOAD_STORE_UNIT_RESERVATION_STATION_H
#define LOAD_STORE_UNIT_RESERVATION_STATION_H

class LoadStoreUnitReservationStation {
    RegisterFile* registerFile;
    LoadStoreUnit* loadStoreUnit;


	int size;
    int head;
    int tail;
	Instruction* instructions;
	int* reorderBufferIndexes;

	int opcode;
	int* operands;
	int reorderBufferIndex;

    int allowLoadReorderBufferIndex;

public:
	LoadStoreUnitReservationStation(RegisterFile* registerFile, LoadStoreUnit* loadStoreUnit) : 
	registerFile(registerFile),
	loadStoreUnit(loadStoreUnit),
	size(10),
    head(0),
    tail(0),
	opcode(0),
	reorderBufferIndex(-1),
    allowLoadReorderBufferIndex(-1)
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
	    if(instructions[tail].opcode != NOOP) {
			if(readyToDispatch(tail)) {
                dispatch(tail);
                reorderBufferIndex = reorderBufferIndexes[tail];
                //printf("DISPATCHING INSTRUCTION: ");
                //Instructions::printInstruction(instructions[i]);
                //printf("WITH REORDER BUFFER INDEX: %d\n", reorderBufferIndex);
                instructions[tail] = (Instruction) {0,0,0,0};
                tail = (tail + 1) % size;
			}
		}
	}

    void setAllowLoadReorderBufferIndex(int i) {
        allowLoadReorderBufferIndex = i;
    }

	void addInstruction(Instruction instruction, int rbi) {
		//printf("ADDED INSTRUCTION: ");
		//Instructions::printInstruction(instruction);
		instructions[head] = instruction;
		reorderBufferIndexes[head] = rbi;
        head = (head + 1) % size;
	}

	int readyToDispatch(int index) {
        Instruction instruction = instructions[index];
		//check that the source register are ready to use
		switch(instruction.opcode) {
			case LW:
                return 1;
                break;
            case SW:
                if(registerFile->getScoreBoardValue(operands[0]) && allowLoadReorderBufferIndex == reorderBufferIndexes[index]) {
                    return 1;
                }
                break;
            case LWR:
                //If the source registers are ready then continue
                if(registerFile->getScoreBoardValue(operands[1])) {
                    return 1;
                }
                break;
            case SWR:
                //If the source registers are ready then continue
                if(registerFile->getScoreBoardValue(operands[0]) && registerFile->getScoreBoardValue(operands[1]) && allowLoadReorderBufferIndex == reorderBufferIndexes[index]) {
                    return 1;
                }
                break;
		}
		return 0;
	}

    //dispatch bound fetch
    void dispatch(int index) {
        Instruction instruction = instructions[index];
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
            case LW:
                break;
            case SW:
                registerNum = operands[0];
                val = registerFile->getRegisterValue(registerNum);
                operands[0] = val;
                break;
            case LWR:
                //If the source registers are ready then continue
                registerNum = operands[1];
                val = registerFile->getRegisterValue(registerNum);
                operands[1] = val;
                break;
            case SWR:
                //If the source registers are ready then continue
                registerNum = operands[0];
                val = registerFile->getRegisterValue(registerNum);
                operands[0] = val;
                registerNum = operands[1];
                val = registerFile->getRegisterValue(registerNum);
                operands[1] = val;
                break;
        }
        //Setting the scoreboard values of the destination register to 0
        switch(opcode) {
            case LW:
            case LWR:
                registerFile->setScoreBoardValue(operands[0],0);
                break;
            case SW:
            case SWR:
                break;
        }
    }

    void pipe() {
    	if(opcode != 0) {

            //send the decoded instruction to the execution unit
            loadStoreUnit->setOpcode(opcode);
            loadStoreUnit->setOperands(operands);

            //Send the reorder buffer index to the execution unit
            loadStoreUnit->setReorderBufferIndex(reorderBufferIndex);
        
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