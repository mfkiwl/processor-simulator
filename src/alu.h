class ALU {
    RegisterFile registerFile;
    public:
        ALU(RegisterFile r) : registerFile(r) {
        	printf("This is the ALU speaking. The Register file has %d registers.\n", registerFile.getNumRegisters());
        }

        void add(int a, int b, int c) {
        	int sum = registerFile.getRegisterValue(b) + registerFile.getRegisterValue(c);
        	registerFile.setRegisterValue(a, sum);
        }
};