#pragma once

#include <shared/common.h>

/**
 * @brief The type of a Value.
 */
typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;

/**
 * @brief Representation of a value from Lox.
 */
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

/**
 * @brief Get the boolean value of a Value.
 * @param value The Value to get the boolean value of
 * @return The boolean value of the Value
 */
#define AS_BOOL(value) ((value).as.boolean)
/**
 * @brief Get the number value of a Value.
 * @param value The Value to get the number value of
 * @return The number value of the Value
 */
#define AS_NUMBER(value) ((value).as.number)

/**
 * @brief Create a Value with a boolean value.
 * @param value The boolean value to create the Value with
 * @return The created Value
 */
#define BOOL_VAL(value) ((Value){ VAL_BOOL, { .boolean = value } })
/**
 * @brief Create a Value with a nil value. Nil is represented as a number 0.
 * @return The created Value
 */
#define NIL_VAL ((Value){ VAL_NIL, { .number = 0 } })
/**
 * @brief Create a Value with a number value.
 * @param value The number value to create the Value with
 * @return The created Value
 */
#define NUMBER_VAL(value) ((Value){ VAL_NUMBER, { .number = value } })

/**
 * @brief Check if a Value is a boolean.
 * @param value The Value to check
 * @return Whether the Value is a boolean
*/
#define IS_BOOL(value)    ((value).type == VAL_BOOL)
/**
 * @brief Check if a Value is nil.
 * @param value The Value to check
 * @return Whether the Value is nil
 */
#define IS_NIL(value)     ((value).type == VAL_NIL)
/**
 * @brief Check if a Value is a number.
 * @param value The Value to check
 * @return Whether the Value is a number
 */
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)


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
 * @brief Check if two Values are equal.
 * @param a The first Value to compare
 * @param b The second Value to compare
 * @return Whether the two Values are equal
 */
bool valuesEqual(Value a, Value b);

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
