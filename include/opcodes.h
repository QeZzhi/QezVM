#ifndef OPCODES_H
#define OPCODES_H

typedef enum {
    OP_PUSH  = 0,
    OP_LOAD  = 1,
    OP_STORE = 2,
    OP_ADD   = 3,
    OP_SUB   = 4,
    OP_MUL   = 5,
    OP_DIV   = 6,
    OP_PRINT = 7,
    OP_PRINT_STR = 8,
    OP_HALT  = 9     
} OpCode;

#endif