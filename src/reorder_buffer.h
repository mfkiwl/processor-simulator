class ReorderBuffer {
	//point to the start and end of the buffer
    int head;
    int tail;

    //size of the buffer
    int size;

    //the buffer
    //1st entry is the status of the instruction (issued, executing, finished)
    //2nd entry is the type of instruction (jump, store to memory, store to register)
    //3rd entry is the destination (either memory address or register number)
    //4th entry is the value that goes to the destination
    int** buffer;

    //store the orignal instructions
    Instruction* DEBUG_instructions;

public:
    ReorderBuffer(int size) : 
        size(size),
        head(0), 
        tail(0)
    {
        //dynamically allocating 2d array to the buffer
        buffer = new int*[size];
        for(int i = 0; i < size; i++) {
        	buffer[i] = new int[4];
        }
        //initialise all entries to zero
        for(int i = 0; i < size; i++) {
        	for(int j = 0; j < 3; j++) {
        		buffer[i][j] = 0;
        	}
        }
        //allocate memory to the instructions buffer
        DEBUG_instructions = new Instruction[size];
    }

    void addToBuffer(Instruction i) {
    	//add the original instruction
    	DEBUG_instructions[head] = i;
    	//get the type of instruction
    	Type t = getInstructionType(i.opcode);
    
    }

    Type getInstructionType(int opcode) {
    	Type t;
    	switch(opcode) {
    		case NOOP:
    		    t = noop;
    		    break;
        	case ADD:
        	case ADDI:
        	case AND:
        	case MULT:
        	case OR:
        	case SUB:
        	case LW:
        	case LWR:
        	    t = storeToRegister;
        	    break;
        	case SW:
        	case SWR:
        	    t = storeToMemory;
        	    break;
        	case BEQ:
        	case BGEZ:
        	case BGTZ:
        	case BLEZ:
        	case BLTZ:
        	case BNE:
        	case J:
        	case JR:
        	    t = jump;
        	    break;
        	case 19:
        	    t = end;
        	    break;
        }
    }
};