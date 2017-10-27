class BranchUnit {
	
    RegisterFile registerFile;

    public:
        BranchUnit(RegisterFile r) : registerFile(r) {}

        void B(int x) {
        	printf("B %d\n", x);
        	registerFile.setpc(x);
        }
};