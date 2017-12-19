#ifndef _ALU_INCLUDED_
#define _ALU_INCLUDED_

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

    //position in the reorder buffer
    int nextReorderBufferIndex;
    int currentReorderBufferIndex;

    public:
        ALU(RegisterFile* registerFile, ReorderBuffer* reorderBuffer) : 
            registerFile(registerFile),
            reorderBuffer(reorderBuffer),
            opcode(0),
            destinationRegister(-1),
            result(0)
        {
            for(int i = 0; i < 3; i++) {
                operands[i] = 0;
            }
        }

        void execute() {
            if(opcode != 0) {
                //tell reorder buffer that we are executing the instruction
                reorderBuffer->executingEntry(nextReorderBufferIndex);
                //execute the instruction
                destinationRegister = operands[0];
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

                currentReorderBufferIndex = nextReorderBufferIndex;

                //reset inputs
                opcode = 0;
                for(int i = 0; i < 3; i++) {
                    operands[i] = 0;
                }
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

        void writeResult() {
            if(destinationRegister != -1) {
                //tell the reorder buffer that we are finished executing the instruction
                reorderBuffer->finishedEntry(currentReorderBufferIndex, result);

                //reset variables
                destinationRegister = -1;
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
            nextReorderBufferIndex = -1;
            currentReorderBufferIndex = -1;
            destinationRegister = -1;
            result = 0;
        }

        void setReorderBufferIndex(int i) {
            nextReorderBufferIndex = i;
        }
};

#endif