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

    void run() {
        //get next instruction
        if(*pc < noOfInstructions) {
    	    nextInstruction = instructions[*pc];
        }
        else {
            nextInstruction = (Instruction) {0,0,0,0};
        }
        //increment pc after fetching the next instruction
        (*pc)++;
    }

    void update() {
        //put the fetched instruction into the instruction register
        decodeUnit->setInstructionRegister(nextInstruction);
    }
};