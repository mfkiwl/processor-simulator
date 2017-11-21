class FetchUnit {
	Instruction *instructions;
	RegisterFile *registerFile;
	DecodeUnit *decodeUnit;

    public:
        FetchUnit(Instruction *instructions, RegisterFile *registerFile, DecodeUnit *decodeUnit) :
        instructions(instructions),
        registerFile(registerFile),
        decodeUnit(decodeUnit)
    {}

    void run() {
    	int pc = registerFile->getpc();
    	Instruction nextInstruction = instructions[pc];
    	decodeUnit->setInstruction(nextInstruction);
    }
};