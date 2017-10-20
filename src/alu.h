class ALU {
    RegisterFile registerFile;
    public:
        ALU(RegisterFile r) : registerFile(r) {
        	printf("This is the ALU speaking. The Register file has %d registers.\n", registerFile.getNumRegisters());
        }

        void ADD(int a, int b, int c) {
        	int value = registerFile.getRegisterValue(b) + registerFile.getRegisterValue(c);
        	registerFile.setRegisterValue(a, value);
        }

        void ADDI(int a, int b, int c) {
        	int value = registerFile.getRegisterValue(b) + c;
        	registerFile.setRegisterValue(a, value);
        }

        void SUB(int a, int b, int c) {
        	int value = registerFile.getRegisterValue(b) - registerFile.getRegisterValue(c);
        	registerFile.setRegisterValue(a, value);
        }
};