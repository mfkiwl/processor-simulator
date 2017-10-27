class Instruction {
	public:
        std::string assembly;
        int opcode;
        int operands[3];
    	Instruction(std::string a, int b, int c, int d, int e) : assembly(a), opcode(b) {
    		operands[0] = c;
    		operands[1] = d;
    		operands[2] = e;
    	}
};