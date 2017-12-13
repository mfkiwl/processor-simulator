class BranchUnit {
	
    //decoded instruction
    int opcode;
    int* operands;

    //set pc program to jump to a position in the program
    int* pc;

    //set to one to send message to processor to flush the pipeline
    int* flushFlag;
    int* runningFlag;

    //no of total instruction executed by the processor
    int* noOfInstructionsExecuted;

    //for debugging purposes
    Instruction DEBUG_Instruction;

    public:
        BranchUnit(int* pc, int* flushFlag, int* runningFlag, int* noOfInstructionsExecuted) : 
            pc(pc),
            flushFlag(flushFlag),
            runningFlag(runningFlag),
            noOfInstructionsExecuted(noOfInstructionsExecuted),
            opcode(0)
        {}

        void execute() {
            if(opcode != 0) {
                //execute the instruction
                switch(opcode) {
                    //BEQ
                    case 11:
                        if(operands[0] == operands[1]) {
                            *pc = operands[2];
                            *flushFlag = 1;
                        }
                        break;
                    //BNE
                    case 16:
                        if(operands[0] != operands[1]) {
                            *pc = operands[2];
                            *flushFlag = 1;
                        }
                        break;
                    //J
                    case 17:
                        *pc = operands[0];
                        *flushFlag = 1;
                         break;
                    //END
                    case 19:
                        //tell the processor that the program had finished
                        *runningFlag = 0;
                        break;
                }

                printf("opcode: %d\n", opcode);
                printf("operands: %d %d %d\n", operands[0], operands[1], operands[2]);

                //increment the number of instructions executed
                (*noOfInstructionsExecuted) += 1;

                //print the instruction that has been executed
                cout << "Executed instruction: ";
                printInstruction(DEBUG_Instruction);

                //reset variables
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
                DEBUG_Instruction = (Instruction) {0,0,0,0};
            }
        }

        void setOpcode(int x) {
            opcode = x;
        }

        void setOperands(int* x) {
            operands = x;
        }

        void set_DEBUG_Instruction(Instruction i) {
            DEBUG_Instruction = i;
        }

        void flush() {
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
            //reset debug instruciton
            DEBUG_Instruction = (Instruction) {0,0,0,0};
        }
};
