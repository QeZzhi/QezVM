#include "../include/qezvm.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s [-v] <file.qc>\n", argv[0]);
        return 1;
    }

    int verbose = 0;
    const char* filename = NULL;

    // Простой парсинг аргументов
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        printf("Error: No input file provided.\n");
        return 1;
    }

    QezVM vm;
    qezvm_init(&vm);

    // Передаем флаг в загрузчик
    if (qezvm_load_file(&vm, filename, verbose) != 0) {
        return 1;
    }

    if (verbose) printf("Starting VM...\n");
    
    // Запускаем с флагом
    int result = qezvm_run(&vm, verbose);
    
    qezvm_free(&vm);
    return result;
}