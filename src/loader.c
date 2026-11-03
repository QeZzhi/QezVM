#include "../include/qezvm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int qezvm_load_file(QezVM* vm, const char* filename, int verbose) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        if (verbose) printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    uint32_t magic;
    fread(&magic, sizeof(uint32_t), 1, file);
    if (magic != QEZVM_MAGIC) {
        if (verbose) printf("Error: Invalid magic bytes. Not a .qc file.\n");
        fclose(file);
        return -1;
    }

    uint32_t count;
    fread(&count, sizeof(uint32_t), 1, file);

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    long code_size = file_size - 8;
    fseek(file, 8, SEEK_SET);

    vm->code_size = code_size;
    vm->code = (uint8_t*)malloc(code_size);
    if (!vm->code) {
        if (verbose) printf("Error: Out of memory\n");
        fclose(file);
        return -1;
    }

    fread(vm->code, 1, code_size, file);
    fclose(file);
    
    // Убрали лишний вывод в консоль, оставили только для отладки
    if (verbose) {
        printf("Loaded %s: %ld bytes (code size)\n", filename, code_size);
    }
    return 0;
}