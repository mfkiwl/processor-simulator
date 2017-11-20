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
    	printf("%p\n", next.operands);
    	for(int i = 0; i < 3; i++) {
    		printf("%d ", next.operands[i]);
    	}
    	printf("\n");
    	printf("fetch unit has run.\n");
    }

    Instruction getNextInstruction() {
    	printf("%p\n", next.operands);
    	for(int i = 0; i < 3; i++) {
    		printf("%d ", next.operands[i]);
    	}
    	printf("\n");
    	return next;
    }
};