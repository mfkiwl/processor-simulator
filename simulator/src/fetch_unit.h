class FetchUnit {
	Instruction *instructions;
	RegisterFile registerFile;
    public:
        FetchUnit(Instruction *instructions, RegisterFile registerFile) :
        instructions(instructions),
        registerFile(registerFile)
        {}
};