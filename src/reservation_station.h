class ReservationStation {
    RegisterFile* registerFile;

	int size;
	Instruction* buffer;

public:
	ReservationStation(RegisterFile* registerFile) : 
	registerFile(registerFile),
	size(10) 
	{
		buffer = new Instruction[size];
		for(int i = 0; i < size; i++) {
			buffer[i] = (Instruction) {0,0,0,0};
		}
	}
};