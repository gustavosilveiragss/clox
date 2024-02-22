#include <sysexits.h>
#include <shared/common.h>
#include <shared/Chunk.h>
#include <shared/Debug.h>
#include <shared/VM.h>

/// @brief Main REPL loop.
static void repl() {
    char line[1024]; // :(
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

/**
 * @brief Reads a file and returns its contents.
 * @param path The path to the file.
 * @return The contents of the file.
 */
static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(EX_IOERR);
    }

    // Go to the end of the file to get its size.
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1); // +1 for null terminator.
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(EX_IOERR);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(EX_IOERR);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

/**
 * @brief Interprets and runs a file of Lox code, given a path.
 * @param path The path to the file.
 */
static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(EX_NOINPUT);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(EX_SOFTWARE);
}

int main(int argc, const char** argv) {
    initVM();

    if (argc) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: lox [path]\n");
        exit(EX_USAGE);
    }

    freeVM();
    return 0;
}
