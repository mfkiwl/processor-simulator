class FetchUnit {
	Instruction* instructions;
	int* pc;
	DecodeUnit *decodeUnit;
    Instruction nextInstruction;

    public:
        FetchUnit(Instruction* instructions, int* pc, DecodeUnit* decodeUnit) :
        instructions(instructions),
        pc(pc),
        decodeUnit(decodeUnit),
        nextInstruction((Instruction) {0,0,0,0})
    {}

    void run() {
        //get next instruction
    	nextInstruction = instructions[*pc];
        //increment pc
        (*pc)++;
        decodeUnit->setInstruction(nextInstruction);
    }
};