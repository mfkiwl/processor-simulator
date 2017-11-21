class FetchUnit {
	Instruction *instructions;
	RegisterFile *registerFile;
	Instruction next;

    public:
        FetchUnit(Instruction *instructions, RegisterFile *registerFile) :
        instructions(instructions),
        registerFile(registerFile)
    {}

    void run() {
    	int pc = registerFile->getpc();
    	next = instructions[pc];
    }

    Instruction getNextInstruction() {
    	return next;
    }
};