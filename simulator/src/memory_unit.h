class MemoryUnit {
	Memory *memory;
	RegisterFile *registerFile;
    DecodeUnit *decodeUnit;

    public:
        MemoryUnit(Memory *memory, RegisterFile *registerFile, DecodeUnit *decodeUnit) : 
        memory(memory),
        registerFile(registerFile),
        decodeUnit(decodeUnit) 
    {}

    void run() {
    	int opcode = decodeUnit->getOpcode();
    	int *operands = decodeUnit->getOperands();
    	switch(opcode) {
    		case 4:
    		    LD(operands[0], operands[1], operands[2]);
    		    break;
    		case 5:
    		    STR(operands[0], operands[1], operands[2]);
    		    break;
    	}
    }

    private:

        void LD(int a, int b, int c) {
            printf("LD R%d R%d %d\n", a, b, c);
            int absoluteAddress = b + c;
            int value = memory->loadFromMemory(absoluteAddress);
            registerFile->setRegisterValue(a, value);
        }

        void STR(int a, int b, int c) {
            printf("STR R%d R%d %d\n", a, b, c);
            int absoluteAddress = b + c;
            int value = registerFile->getRegisterValue(a);
            memory->storeInMemory(absoluteAddress, value);
        }
};