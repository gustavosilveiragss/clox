#pragma once

#include <shared/common.h>
#include <shared/Value.h>

/**
 * @brief Extracts the object type from a Value.
 * @param value The Value to extract the object type from
 * @return The object type of the Value
 */
#define OBJ_TYPE(value) (AS_OBJ(value)->type)

/**
 * @brief Checks if a Value is of type ObjString.
 * @param value The Value to check the object type of
 * @return Whether the Value is of the type ObjString
 */
#define IS_STRING(value) isObjType(value, OBJ_STRING)

/**
 * @brief "Cast" a Value to an ObjString.
 * @param value The Value be casted to an ObjString
 * @return An ObjString from the Value
 */
#define AS_STRING(value) ((ObjString*)AS_OBJ(value))

/**
 * @brief Get the character array of an ObjString.
 * @param value The ObjString to get the character array of
 * @return The character array of the ObjString
 */
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

/**
 * @brief Checks if a Value is of a specific object type.
 * @param value The Value to check the object type of
 * @param type The object type to check for
 * @return Whether the Value is of the specified object type
 */
static inline bool isObjType(Value value, ObjType type) {
    // This was made into a function because if on the macro, value would be evaluated twice
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

/// @brief The type of an object.
typedef enum {
    OBJ_STRING,
} ObjType;

/// @brief Representation of an object from Lox.
struct Obj {
    ObjType type;
};

/// @brief Representation of a string object from Lox.
struct ObjString {
    Obj obj;
    int length;
    char* chars;
};
