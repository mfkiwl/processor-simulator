class ALU {

    RegisterFile* registerFile;
    int opcode;
    int* operands;


    public:
        ALU(RegisterFile* registerFile) : 
            registerFile(registerFile), 
            opcode(0),
            operands(NULL)
        {}

        void run() {
            if(opcode != 0 && operands != NULL) {
                switch(opcode) {
                    case 1:
                        ADD(operands[0], operands[1], operands[2]);
                        break;
                    case 2:
                        ADDI(operands[0], operands[1], operands[2]);
                        break;
                    case 3:
                        SUB(operands[0], operands[1], operands[2]);
                        break;
                }
                opcode = 0;
                operands = NULL;
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int *x) {
            operands = x;
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