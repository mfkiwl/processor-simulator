class ALU {

    //forward components
    RegisterFile* registerFile;
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //instruction result
    int destinationRegister;
    int result;

    //bypassing variables
    int bypassing;
    int bypassingOperand;

    //position in the reorder buffer
    int reorderBufferIndex;

    public:
        ALU(RegisterFile* registerFile, ReorderBuffer* reorderBuffer) : 
            registerFile(registerFile),
            reorderBuffer(reorderBuffer),
            opcode(0),
            destinationRegister(-1),
            result(0),
            bypassing(0),
            bypassingOperand(0)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {
                //execute the instruction
                destinationRegister = operands[0];
                if(bypassing) {
                    operands[bypassingOperand] = result;
                }
                switch(opcode) {
                    case ADD:
                    case ADDI:
                        result = operands[1] + operands[2];
                        break;
                    case AND:
                        result = operands[1] && operands[2];
                        break;
                    case MULT:
                        result = operands[1] * operands[2];
                        break;
                    case OR:
                        result = operands[1] || operands[2];
                        break;
                    case SUB:
                        result = operands[1] - operands[2];
                        break;
                }
            }
            //reset inputs
            opcode = 0;
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        int canBypass(int r) {
            if(r == destinationRegister) {
                return 1;
            }
            else {
                return 0;
            }
        }

        void writeback() {
            if(destinationRegister != -1) {
                //write the result to the output register
                registerFile->setRegisterValue(destinationRegister, result);
                //Set the scoreBoard of the destination register to 1
                registerFile->setScoreBoardValue(destinationRegister, 1);

                //increment the number of instructions executed
                //(*noOfInstructionsExecuted) += 1;

                //reset variables
                destinationRegister = -1;
            }
        }

        void setBypassing(int a, int b) {
            bypassing = a;
            bypassingOperand = b;
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
