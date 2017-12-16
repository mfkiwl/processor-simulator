enum status {issued, executing, finished};
enum type {jump, storeToMemory, storeToRegister, noop, end};

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
    	type t = getInstructionType(i.opcode);
    }

    type getInstructionType(int opcode) {
    	type t;
    	switch(opcode) {
    		case 0:
    		    t = noop;
    		    break;
        	case 1:
        	case 2:
        	case 3:
        	case 4:
        	case 5:
        	case 6:
        	case 7:
        	case 8:
        	    t = storeToRegister;
        	    break;
        	case 9:
        	case 10:
        	    t = storeToMemory;
        	    break;
        	case 11:
        	case 12:
        	case 13:
        	case 14:
        	case 15:
        	case 16:
        	case 17:
        	case 18:
        	    t = jump;
        	    break;
        	case 19:
        	    t = end;
        	    break;
        }
    }
};