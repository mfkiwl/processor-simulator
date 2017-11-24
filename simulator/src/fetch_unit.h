class FetchUnit {
	Instruction* instructions;
    int noOfInstructions;
	int* pc;
	DecodeUnit* decodeUnit;
    Instruction nextInstruction;

    public:
        FetchUnit(Instruction* instructions, int noOfInstructions, int* pc, DecodeUnit* decodeUnit) :
        instructions(instructions),
        noOfInstructions(noOfInstructions),
        pc(pc),
        decodeUnit(decodeUnit),
        nextInstruction((Instruction) {0,0,0,0})
    {}

    void execute() {
        if(*pc < noOfInstructions) {
            //fetch the next instruction
    	    nextInstruction = instructions[*pc];
            //increment the program counter
            (*pc)++;
        }
        else {
            nextInstruction = (Instruction) {0,0,0,0};
        }
    }

    void pipe() {
        //put the fetched instruction into the instruction register
        decodeUnit->setInstructionRegister(nextInstruction);
    }
};