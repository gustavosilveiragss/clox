#pragma once

#include <shared/VM.h>
#include <shared/Scanner.h>

/**
 * @brief Parser for the compiler.
 * @var Parser::current The current token.
 * @var Parser::previous The previous token.
 */
typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

/// @brief Precedence enum for the compiler.
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Precedence;

/// @brief Function type that takes no arguments and returns nothing.
typedef void (*ParseFn)();

/**
 * @brief Parse rule for the compiler. Could be a prefix or infix rule.
 * @var ParseRule::prefix the function to compile a prefix expression starting with a token of that type
 * @var ParseRule::infix the function to compile an infix expression whose left operand is followed by a token of that type
 * @var ParseRule::precedence the precedence of an infix expression that uses that token as an operator
 */
typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

/**
 * @brief Scans, parses, and compiles the source code.
 * @param source The source code to compile.
 * @param chunk The Chunk to put the bytecode into.
 */
bool compile(const char* source, Chunk* chunk);
