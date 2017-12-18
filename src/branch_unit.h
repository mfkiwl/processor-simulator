class BranchUnit {
	
    //decoded instruction
    int opcode;
    int operands[3];

    //set pc program to jump to a position in the program
    int* pc;

    //set to one to send message to processor to flush the pipeline
    int* flushFlag;
    int* runningFlag;

    //for debugging purposes
    Instruction nextInstruction;

    public:
        BranchUnit(int* pc, int* flushFlag, int* runningFlag) : 
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

                //print the instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(nextInstruction);

                //reset variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                nextInstruction = (Instruction) {0,0,0,0};
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

        void setNextInstruction(Instruction i) {
            nextInstruction = i;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //reset debug instruciton
            nextInstruction = (Instruction) {0,0,0,0};
        }
};
