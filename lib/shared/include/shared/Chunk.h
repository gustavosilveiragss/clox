#pragma once

#include <shared/common.h>
#include <shared/Value.h>

/// @brief Operation codes for instructions in the bytecode
typedef enum {
    OP_NOT,
    OP_CONSTANT,
    OP_ADD,
    OP_SUBTRACT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

/**
 * @brief A list of bytecode instructions
 * @var Chunk::count The current number of instructions in the list
 * @var Chunk::capacity The number of instructions the list can hold
 * @var Chunk::code The list of instructions
 * @var Chunk::constants The Constant Pool used by the instructions
 * @var Chunk::lines The line numbers of the instructions
 */
typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    int* lines;
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
 * @param line The line number of the instruction
 */
void writeChunk(Chunk* chunk, uint8_t byte, int line);

/**
 * @brief Add a constant to the constant pool
 * @param chunk The chunk to add the constant to
 * @param value The constant to add
 * @return The index of the constant in the constant pool
 */
int addConstant(Chunk* chunk, Value value);
