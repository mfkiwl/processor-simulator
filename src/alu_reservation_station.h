class ALUReservationStation {
    RegisterFile* registerFile;
    ALU* alu;

	int size;
	Instruction* instructions;

	Instruction dispatchingInstruction;

	int opcode;
	int* operands;

public:
	ALUReservationStation(RegisterFile* registerFile, ALU* alu) : 
	registerFile(registerFile),
	alu(alu),
	size(10),
	opcode(0)
	{
		//allocate memory to the buffer
		instructions = new Instruction[size];
		//inialise all instructions to NOOPs
		for(int i = 0; i < size; i++) {
			instructions[i] = (Instruction) {0,0,0,0};
		}
		//allocate memory for the operands array
		operands = new int[3];
	}

    //dispatch an instruction if it can
	void execute() {
		for(int i = 0; i < size; i++) {
			if(instructions[i].opcode != NOOP) {
				if(readyToDispatch(instructions[i])) {
                    dispatch(instructions[i]);
                    instructions[i] = (Instruction) {0,0,0,0};
                    break;
				}
			}
		}
	}

	void addToReservationStation(Instruction instruction) {
		instructions[size - 1] = instruction;
	}

	int readyToDispatch(Instruction instruction) {
		switch(instruction.opcode) {
			case ADD:
			case AND:
			case MULT:
			case OR:
			case SUB:
			    if(registerFile->getScoreBoardValue(instruction.operands[1]) && registerFile->getScoreBoardValue(instruction.operands[2])) {
			    	return 1;
			    }
			    break;
			case ADDI:
			    if(registerFile->getScoreBoardValue(instruction.operands[1])) {
			    	return 1;
			    }
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
            case ADD:
            case AND:
            case MULT:
            case OR:
            case SUB:
                for(int i = 1; i < 3; i++) {
                    registerNum = operands[i];
                    val = registerFile->getRegisterValue(registerNum);
                    operands[i] = val;
                }
                break;
            case ADDI:
                registerNum = operands[1];
                val = registerFile->getRegisterValue(registerNum);
                operands[1] = val;
                break;
        }
    }

    void pipe() {
    	//send the decoded instruction to the execution unit
        alu->setOpcode(opcode);
        alu->setOperands(operands);
        //Setting the scoreBoard values of the destination register to 0
        registerFile->setScoreBoardValue(operands[0],0);
        //Instruction has been issued so add entry to the reorder buffer
        
        //reset the opcode and operands
        opcode = 0;
        for(int i = 0; i < 3; i++) {
            operands[i] = 0;
        }
    }
};