#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Operation codes for instructions in the bytecode
 * @var OpCode::OP_RETURN Return
 */
typedef enum {
    OP_RETURN,
} OpCode;
