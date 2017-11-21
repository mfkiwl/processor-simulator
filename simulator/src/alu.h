class ALU {

    RegisterFile *registerFile;
    DecodeUnit *decodeUnit;
    
    public:
        ALU(RegisterFile *registerFile, DecodeUnit *decodeUnit) : 
            registerFile(registerFile), 
            decodeUnit(decodeUnit)
        {}

        void run() {
            int opcode = decodeUnit->getOpcode();
            int *operands = decodeUnit->getOperands();
            switch(opcode) {
                case 0:
                    ADD(operands[0], operands[1], operands[2]);
                    break;
                case 1:
                    ADDI(operands[0], operands[1], operands[2]);
                    break;
                case 2:
                    SUB(operands[0], operands[1], operands[2]);
                    break;
            }
        }

    private:
        void ADD(int a, int b, int c) {
        	printf("ADD R%d R%d R%d\n", a, b, c);
        	int value = registerFile->getRegisterValue(b) + registerFile->getRegisterValue(c);
        	registerFile->setRegisterValue(a, value);
        }

        void ADDI(int a, int b, int c) {
        	printf("ADDI R%d R%d %d\n", a, b, c);
        	int value = registerFile->getRegisterValue(b) + c;
        	registerFile->setRegisterValue(a, value);
        }

        void SUB(int a, int b, int c) {
        	printf("SUB R%d R%d R%d\n", a, b, c);
        	int value = registerFile->getRegisterValue(b) - registerFile->getRegisterValue(c);
        	registerFile->setRegisterValue(a, value);
        }
};