//===========================
//include guard
#ifndef CONSTANTS_H
#define CONSTANTS_H

//instruction opcodes
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
  HALT
};

//type in the reorder buffer
enum Type {
  JUMP,
  STORE_TO_MEMORY,
  STORE_TO_REGISTER,
  SYSCALL
};

//index in the reorder buffer
enum Index {
  TYPE,
  STATUS,
  RESULT,
  BRANCH_PREDICTION,
  BRANCH_TARGET_ADDRESS,
  ARCHITECTURAL_REGISTER,
  PHYSICAL_REGISTER,
  PREVIOUS_PHYSICAL_REGISTER
};

//status in the reorder buffer
enum Status {
  EMPTY,
  ISSUED,
  EXECUTING,
  FINISHED
};

enum OperandTypes {
  NONE,
  REGISTER,
  ROB,
  CONSTANT
};

#endif