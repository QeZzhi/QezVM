#include "../include/qezvm.h"
#include "../include/opcodes.h"
#include <stdio.h>
#include <stdlib.h>

void qezvm_init(QezVM* vm) {
    vm->code = NULL;
    vm->code_size = 0;
    vm->ip = 0;
    vm->sp = 0;
}

int qezvm_run(QezVM* vm, int verbose) {
    while (vm->ip < vm->code_size) {
        // Читаем 1 байт (опкод)
        uint8_t op = vm->code[vm->ip++];
        
        switch (op) {
            case OP_HALT:
                // Выводим сообщение только если включена отладка
                if (verbose) printf("VM Halted.\n");
                return 0;

            case OP_PUSH: {
                if (vm->ip + 4 > vm->code_size) {
                    if (verbose) printf("Error: Unexpected end of bytecode in PUSH\n");
                    return -1;
                }
                int32_t value = 0;
                value |= (int32_t)vm->code[vm->ip + 0] << 0;
                value |= (int32_t)vm->code[vm->ip + 1] << 8;
                value |= (int32_t)vm->code[vm->ip + 2] << 16;
                value |= (int32_t)vm->code[vm->ip + 3] << 24;
                vm->ip += 4;
                
                if (vm->sp >= 256) {
                    if (verbose) printf("Error: Stack overflow\n");
                    return -1;
                }
                vm->stack[vm->sp++] = value; 
                break;
            }

            case OP_LOAD: {
                if (vm->ip + 4 > vm->code_size) return -1;
                uint32_t len = 0;
                len |= (uint32_t)vm->code[vm->ip + 0] << 0;
                len |= (uint32_t)vm->code[vm->ip + 1] << 8;
                len |= (uint32_t)vm->code[vm->ip + 2] << 16;
                len |= (uint32_t)vm->code[vm->ip + 3] << 24;
                vm->ip += 4 + len;
                break;
            }

            case OP_STORE: {
                if (vm->ip + 4 > vm->code_size) return -1;
                uint32_t len = 0;
                len |= (uint32_t)vm->code[vm->ip + 0] << 0;
                len |= (uint32_t)vm->code[vm->ip + 1] << 8;
                len |= (uint32_t)vm->code[vm->ip + 2] << 16;
                len |= (uint32_t)vm->code[vm->ip + 3] << 24;
                vm->ip += 4 + len;
                break;
            }

            case OP_ADD: {
                if (vm->sp < 2) { if (verbose) printf("Error: Stack underflow on ADD\n"); return -1; }
                int32_t b = vm->stack[--vm->sp];
                int32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a + b;
                break;
            }

            case OP_SUB: {
                if (vm->sp < 2) { if (verbose) printf("Error: Stack underflow on SUB\n"); return -1; }
                int32_t b = vm->stack[--vm->sp];
                int32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a - b;
                break;
            }

            case OP_MUL: {
                if (vm->sp < 2) { if (verbose) printf("Error: Stack underflow on MUL\n"); return -1; }
                int32_t b = vm->stack[--vm->sp];
                int32_t a = vm->stack[--vm->sp];
                vm->stack[vm->sp++] = a * b;
                break;
            }

            case OP_DIV: {
                if (vm->sp < 2) { if (verbose) printf("Error: Stack underflow on DIV\n"); return -1; }
                int32_t b = vm->stack[--vm->sp];
                int32_t a = vm->stack[--vm->sp];
                if (b == 0) { if (verbose) printf("Error: Division by zero\n"); return -1; }
                vm->stack[vm->sp++] = a / b;
                break;
            }

            case OP_PRINT: {
                if (vm->sp < 1) { if (verbose) printf("Error: Stack underflow on PRINT\n"); return -1; }
                int32_t val = vm->stack[--vm->sp];
                // Убрали \n, чтобы числа можно было писать в строку без переноса
                printf("%d", val); 
                break;
            }

            case OP_PRINT_STR: {
                if (vm->ip + 4 > vm->code_size) return -1;
                uint32_t len = 0;
                len |= (uint32_t)vm->code[vm->ip + 0] << 0;
                len |= (uint32_t)vm->code[vm->ip + 1] << 8;
                len |= (uint32_t)vm->code[vm->ip + 2] << 16;
                len |= (uint32_t)vm->code[vm->ip + 3] << 24;
                vm->ip += 4;
                
                for (uint32_t i = 0; i < len; i++) {
                    printf("%c", vm->code[vm->ip + i]);
                }
                // Убрали \n для слитного вывода строк
                vm->ip += len; 
                break;
            }

            default:
                if (verbose) printf("Error: Unknown opcode 0x%02X at IP %zu\n", op, vm->ip - 1);
                return -1;
        }
    }
    return 0;
}

void qezvm_free(QezVM* vm) {
    if (vm->code) {
        free(vm->code);
        vm->code = NULL;
    }
}