class MemoryUnit {
	Memory memory;
	RegisterFile registerFile;
    DecodeUnit decodeUnit;

    public:
        MemoryUnit(Memory memory, RegisterFile registerFile, DecodeUnit decodeUnit) : 
        memory(memory),
        registerFile(registerFile),
        decodeUnit(decodeUnit) 
        {}

    void LD(int a, int b, int c) {
        printf("LD R%d R%d %d\n", a, b, c);
        int absoluteAddress = b + c;
        int value = memory.loadFromMemory(absoluteAddress);
        registerFile.setRegisterValue(a, value);
    }

    void STR(int a, int b, int c) {
        printf("STR R%d R%d %d\n", a, b, c);
        int absoluteAddress = b + c;
        int value = registerFile.getRegisterValue(a);
        memory.storeInMemory(absoluteAddress, value);
    }
};