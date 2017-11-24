class MemoryUnit {
	Memory* memory;
	RegisterFile* registerFile;
    int opcode;
    int* operands;

    public:
        MemoryUnit(Memory* memory, RegisterFile* registerFile) : 
            memory(memory),
            registerFile(registerFile),
            opcode(0),
            operands(NULL)
        {}

        void run() {
            if(opcode != 0 && operands != NULL) {
    	        switch(opcode) {
    		        case 5:
    		            LD(operands[0], operands[1], operands[2]);
    		            break;
    		        case 6:
    		            STR(operands[0], operands[1], operands[2]);
    		            break;
                }
                opcode = 0;
                operands = NULL;
    	    }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int* x) {
            operands = x;
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