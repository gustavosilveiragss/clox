#pragma once

#include <shared/Chunk.h>
#include <shared/Value.h>

/// @brief Maximum number of values that can be stored on the stack.
#define STACK_MAX 256

/**
 * @brief Virtual Machine struct.
 * @var VM::chunk The Chunk to execute.
 * @var VM::ip The current instruction pointer.
 * @var VM::stack The stack of Values.
 * @var VM::stackTop The top of the stack.
 */
typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

/**
 * @brief Result of interpreting a Chunk.
 * @var InterpretResult::INTERPRET_OK The Chunk was interpreted successfully.
 * @var InterpretResult::INTERPRET_COMPILE_ERROR The Chunk failed to compile.
 * @var InterpretResult::INTERPRET_RUNTIME_ERROR The Chunk failed to run.
 */
typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

/**
 * @brief Initializes the Virtual Machine.
 */
void initVM();

/**
 * @brief Frees the Virtual Machine.
 */
void freeVM();

/**
 * @brief Interprets a Chunk.
 * @param chunk The Chunk to interpret.
 * @return The result of interpreting the Chunk.
 */
InterpretResult interpret(Chunk* chunk);

/**
 * @brief Pushes a Value onto the stack.
 * @param value The Value to push.
 */
void push(Value value);

/**
 * @brief Pops a Value off the stack.
 * @return The Value that was popped.
 */
Value pop();