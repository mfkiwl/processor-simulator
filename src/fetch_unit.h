#ifndef FETCH_UNIT_H
#define FETCH_UNIT_H

class FetchUnit {
    Instructions instructions;
    int* pc;
    DecodeIssueUnit* decodeIssueUnit;
    Instruction currentInstruction;

    public:
        FetchUnit(Instructions instructions, int* pc, DecodeIssueUnit* decodeIssueUnit) :
        instructions(instructions),
        pc(pc),
        decodeIssueUnit(decodeIssueUnit),
        currentInstruction((Instruction) {0,0,0,0})
    {}

    void execute() {
        if(*pc <= instructions.getNumOfInstructions()) {
            //fetch the next instruction (-1 so that pc of 1 refers to the first instruction on line 1)
    	    currentInstruction = instructions.at(*pc - 1);
            //increment the program counter
            (*pc)++;
        }
        else {
            //next instruction is noop if pc exceeds number of instructions
            currentInstruction = (Instruction) {0,0,0,0};
        }
    }

    void pipe() {
        //put the fetched instruction into the instruction register
        decodeIssueUnit->setNextInstruction(currentInstruction);
    }

    void flush() {
        currentInstruction = (Instruction) {0,0,0,0};
    }
};

#endif