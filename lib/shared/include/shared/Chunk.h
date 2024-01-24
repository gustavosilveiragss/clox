#pragma once

#include <shared/common.h>
#include <shared/Value.h>

/**
 * @brief Operation codes for instructions in the bytecode
 * @var OpCode::OP_CONSTANT Constant
 * @var OpCode::OP_RETURN Return
 */
typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;

/**
 * @brief A list of bytecode instructions
 * @var Chunk::count The current number of instructions in the list
 * @var Chunk::capacity The number of instructions the list can hold
 * @var Chunk::code The list of instructions
 * @var Chunk::constants The Constant Pool used by the instructions
 */
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
} Chunk;

/**
 * @brief Initialize a chunk of bytecode, with a capacity of 0
 * @param chunk The chunk to initialize
 */
void initChunk(Chunk* chunk);

/**
 * @brief Free a bytecode chunk
 * @param chunk The chunk to free
 */
void freeChunk(Chunk* chunk);

/**
 * @brief Write a byte to the bytecode chunk
 * @param chunk The chunk to write to
 * @param byte The byte to write
 */
void writeChunk(Chunk* chunk, uint8_t byte);

/**
 * @brief Add a constant to the constant pool
 * @param chunk The chunk to add the constant to
 * @param value The constant to add
 * @return The index of the constant in the constant pool
 */
int addConstant(Chunk* chunk, Value value);