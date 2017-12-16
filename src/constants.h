enum Opcodes {
    NOOP,
    ADD,
    ADDI,
    AND,
    MULT,
    OR,
    SUB,
    LW,
    LWR,
    SW,
    SWR,
    BEQ,
    BGEZ,
    BGTZ,
    BLEZ,
    BLTZ,
    BNE,
    J,
    JR,
    END
};

enum Index {
    status,
    type,
    destination,
    value
};

enum Status {
    issued,
    executing,
    finished
};

enum Type {
    jump,
    storeToMemory,
    storeToRegister,
    noop,
    end
};