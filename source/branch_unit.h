#ifndef BRANCH_UNIT_H
#define BRANCH_UNIT_H

class BranchUnit {
	
    ReorderBuffer* reorderBuffer;

    //decoded instruction
    int opcode;
    int operands[3];

    //position in the reorder buffer
    int reorderBufferIndex;

    //if the branch condition is met or not
    int successful;

  public:
    BranchUnit(ReorderBuffer* reorderBuffer) : 
      reorderBuffer(reorderBuffer),
      opcode(0),
      reorderBufferIndex(-1),
      successful(0)
    {
      for(int i = 0; i < 3; i++) {
        operands[i] = 0;
      }
    }

    void execute() {
      if(reorderBufferIndex != -1) {
        //tell reorder buffer that we are executing the instruction
        /*
        printf("\n\n");
        printf("REORDER BUFFER INDEX: %d\n", reorderBufferIndex);
        printf("opcode: %d\n", opcode);
        printf("operands: %d %d %d\n", operands[0], operands[1], operands[2]);
        printf("\n\n");
        */
        reorderBuffer->executingEntry(reorderBufferIndex);
        //execute the instruction
        switch(opcode) {
          case BEQ:
            if(operands[0] == operands[1]) {
              successful = 1;
              //*pc = operands[2];
              //*flushFlag = 1;
            }
            break;
          case BNE:
            if(operands[0] != operands[1]) {
              successful = 1;
              //*pc = operands[2];
              //*flushFlag = 1;
            }
            break;
          case J:
            successful = 1;
            //*pc = operands[0];
            //*flushFlag = 1;
            break;
          case HALT:
            successful = 1;
            //tell the processor that the program had finished
            //*runningFlag = 0;
            break;
        } 

        //reset variables
        opcode = 0;
        for(int i = 0; i < 3; i++) {
          operands[i] = 0;
        }
        printf("\n\nEXECUTED BRANCH UNIT\n\n\n");
      }
    }

    void pipe() {
      if(reorderBufferIndex != -1) {
        //tell the reorder buffer that we are finished executing the instruction
        reorderBuffer->finishedEntry(reorderBufferIndex, successful);
        opcode = 0;
        for(int i = 0; i < 3; i++) {
          operands[i] = 0;
        }
        reorderBufferIndex = -1;
        successful = 0;
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
      reorderBufferIndex = -1;
    }

    void setReorderBufferIndex(int i) {
      reorderBufferIndex = i;
    }
};

#endif