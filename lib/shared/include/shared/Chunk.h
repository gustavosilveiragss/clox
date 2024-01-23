#pragma once

#include <shared/common.h>
#include <shared/Memory.h>

/**
 * @brief A dynamic length list of bytecode instructions
 * @var Chunk::count The current number of elements in the list
 * @var Chunk::capacity The number of elements the list can hold
 * @var Chunk::code The list of elements
 */
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
} Chunk;

/**
 * @brief Initialize a chunk of bytecode, with 
 * @param chunk
 */
void initChunk(Chunk* chunk);

/**
 * @brief Free a dynamic length list
 * @param chunk
 */
void freeChunk(Chunk* chunk);

/**
 * @brief Write a byte to the list
 * @param chunk
 * @param byte
 */
void writeChunk(Chunk* chunk, uint8_t byte);