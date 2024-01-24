#pragma once

#include <shared/common.h>

/**
 * @brief Representation of a value from Lox.
 */
typedef double Value;

/**
 * @brief A list for literal values. Used for the constant pool.
 * @var ValueArray::count The number of values currently stored in the list.
 * @var ValueArray::capacity The number of values that can be stored in the list.
 * @var ValueArray::values The list of values.
 */
typedef struct {
    int count;
    int capacity;
    Value* values;
} ValueArray;

/**
 * @brief Initialize a ValueArray, with a capacity of 0.
 * @param array The ValueArray to initialize
 */
void initValueArray(ValueArray* array);

/**
 * @brief Free the memory used by a ValueArray.
 * @param array The ValueArray to free
 */
void freeValueArray(ValueArray* array);

/**
 * @brief Write a Value to the ValueArray.
 * @param array The ValueArray to write to
 * @param value The Value to write
 */
void writeValueArray(ValueArray* array, Value value);
