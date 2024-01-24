#include <shared/common.h>
#include <shared/Chunk.h>
#include <shared/Debug.h>
#include <shared/VM.h>

int main() {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    writeChunk(&chunk, OP_RETURN, 123);

    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);
    return 0;
}