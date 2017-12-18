class BranchUnit {
	
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //set pc program to jump to a position in the program
    int* pc;

    //set to one to send message to processor to flush the pipeline
    int* flushFlag;
    int* runningFlag;

    //position in the reorder buffer
    int reorderBufferIndex;

    public:
        BranchUnit(ReorderBuffer* reorderBuffer, int* pc, int* flushFlag, int* runningFlag) : 
            pc(pc),
            flushFlag(flushFlag),
            runningFlag(runningFlag),
            opcode(0)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {
                //execute the instruction
                switch(opcode) {
                    case BEQ:
                        if(operands[0] == operands[1]) {
                            *pc = operands[2];
                            *flushFlag = 1;
                        }
                        break;
                    case BNE:
                        if(operands[0] != operands[1]) {
                            *pc = operands[2];
                            *flushFlag = 1;
                        }
                        break;
                    case J:
                        *pc = operands[0];
                        *flushFlag = 1;
                         break;
                    case HALT:
                        //tell the processor that the program had finished
                        *runningFlag = 0;
                        break;
                }

                //increment the number of instructions executed
                //(*noOfInstructionsExecuted) += 1;

                //reset variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int x[3]) {
            for(int i = 0; i < 3; i++) {
                operands[i] = x[i];
            }
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void setReorderBufferIndex(int i) {
            reorderBufferIndex = i;
        }
};
