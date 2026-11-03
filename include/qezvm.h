#ifndef QEZVM_H
#define QEZVM_H

#include <stdint.h>
#include <stddef.h>

#define QEZVM_MAGIC 0x515A564D // "QZVM"

typedef struct {
    uint8_t* code;
    size_t code_size;
    size_t ip;
    int32_t stack[256];
    size_t sp;
} QezVM;

void qezvm_init(QezVM* vm);

// Добавили параметр verbose
int qezvm_load_file(QezVM* vm, const char* filename, int verbose);
int qezvm_run(QezVM* vm, int verbose);
void qezvm_free(QezVM* vm);

#endif