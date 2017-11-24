class DecodeUnit {
	ALU* alu;
	BranchUnit* branchUnit;
	MemoryUnit* memoryUnit;
    Instruction instruction;

    public:
    	DecodeUnit(ALU* alu, BranchUnit* branchUnit, MemoryUnit* memoryUnit) :
    	    alu(alu),
    	    branchUnit(branchUnit),
    	    memoryUnit(memoryUnit)
        {}

        void run() {
    	    switch(instruction.opcode) {
    	    	case 1:
    	    	case 2:
    	    	case 3:
    	    	    alu->setOpcode(instruction.opcode);
    	    	    alu->setOperands(instruction.operands);
    	    	    branchUnit->setOpcode(0);
    	    	    branchUnit->setOperands(NULL);
    	    	    memoryUnit->setOpcode(0);
    	    	    memoryUnit->setOperands(NULL);
    	    	    break;
    	    	case 4:
    	    	    alu->setOpcode(0);
    	    	    alu->setOperands(NULL);
    	    	    branchUnit->setOpcode(instruction.opcode);
    	    	    branchUnit->setOperands(instruction.operands);
    	    	    memoryUnit->setOpcode(0);
    	    	    memoryUnit->setOperands(NULL);
                    break;
                case 5:
                case 6:
                    alu->setOpcode(0);
                    alu->setOperands(NULL);
                    branchUnit->setOpcode(0);
                    branchUnit->setOperands(NULL);
                    memoryUnit->setOpcode(instruction.opcode);
                    memoryUnit->setOperands(instruction.operands);
                    break;
    	    }
        }

        void update() {

        }

        void setInstruction(Instruction x) {
        	instruction = x;
        }
};