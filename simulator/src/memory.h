class Memory {
	//the number of bytes the memory has
	int size;
    int* memory;

    public:
        Memory(int size) : size(size) {
        	memory = (int*) malloc(sizeof(int) * size);
        	for(int i = 0; i < size; i++) {
        		memory[i] = 0;
        	}
        }
    
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
};