class ALU {

    RegisterFile registerFile;
    
    public:
        ALU(RegisterFile r) : registerFile(r) {}

        void ADD(int a, int b, int c) {
        	printf("ADD R%d R%d R%d\n", a, b, c);
        	int value = registerFile.getRegisterValue(b) + registerFile.getRegisterValue(c);
        	registerFile.setRegisterValue(a, value);
        }

        void ADDI(int a, int b, int c) {
        	printf("ADDI R%d R%d %d\n", a, b, c);
        	int value = registerFile.getRegisterValue(b) + c;
        	registerFile.setRegisterValue(a, value);
        }

        void SUB(int a, int b, int c) {
        	printf("SUB R%d R%d R%d\n", a, b, c);
        	int value = registerFile.getRegisterValue(b) - registerFile.getRegisterValue(c);
        	registerFile.setRegisterValue(a, value);
        }
};