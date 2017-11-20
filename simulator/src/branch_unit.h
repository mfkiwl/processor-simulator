class BranchUnit {
	
    RegisterFile registerFile;
    DecodeUnit decodeUnit;

    public:
        BranchUnit(RegisterFile registerFile, DecodeUnit decodeUnit) : registerFile(registerFile), decodeUnit(decodeUnit) {}

        void B(int x) {
        	printf("B %d\n", x);
        	registerFile.setpc(x);
        }
};