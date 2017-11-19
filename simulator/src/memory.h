class Memory {
	//the number of bytes the memory has
	int size;
    int* memory;

    RegisterFile registerFile;

    public:
        Memory(int a, RegisterFile r) : size(a), registerFile(r) {
        	memory = (int *) malloc(sizeof(int) * size);
        	for(int i = 0; i < size; i++) {
        		memory[i] = 0;
        	}
        }
    
    private:
        int loadFromMemory(int address) {
        	if(address < 0 || address > size - 1) {
        		printf("Register index %d is out of bounds.\n", address);
        	}
        	return memory[address];
        }

        void storeInMemory(int address, int value) {
        	if(address < 0 || address > size - 1) {
        		printf("Register index %d is out of bounds.\n", address);
        	}
        	memory[address] = value;
        }

    public:
        void LD(int a, int b, int c) {
        	printf("LD R%d R%d %d\n", a, b, c);
        	int absoluteAddress = b + c;
        	int value = loadFromMemory(absoluteAddress);
        	registerFile.setRegisterValue(a, value);
        }

        void STR(int a, int b, int c) {
        	printf("STR R%d R%d %d\n", a, b, c);
        	int absoluteAddress = b + c;
        	int value = registerFile.getRegisterValue(a);
        	storeInMemory(absoluteAddress, value);
        }
};