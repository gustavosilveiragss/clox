#include <shared/common.h>
#include <shared/Compiler.h>
#include <shared/Scanner.h>
#include <shared/VM.h>

Parser parser;
Chunk* compilingChunk;

/// @brief Gets the current chunk being compiled
/// @return The current chunk being compiled
static Chunk* currentChunk() {
    // TODO: Later, when we start compiling user-defined functions, the notion of “current chunk” gets more complicated
    return compilingChunk;
}

/**
 * @brief Reports an error at the given token, with the given message. If the lexeme is readable, it will be included in the error message.
 * @param token The token to report the error at
 * @param message The error message
 */
static void errorAt(Token* token, const char* message) {
    if (parser.panicMode)
        return;
    parser.panicMode = true;
    fprintf(stderr, "[line %d] Error", token->line);

    switch (token->type) {
    case TOKEN_EOF:
        fprintf(stderr, " at end");
        break;
    case TOKEN_ERROR:
        // Nothing.
        break;
    default:
        fprintf(stderr, " at '%.*s'", token->length, token->start);
        break;
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

/**
 * @brief Error message for the current token
 * @param message The error message
 */
static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

/**
 * @brief Error message for the token that was just consumed
 * @param message The error message
 */
static void error(const char* message) {
    errorAt(&parser.previous, message);
}

/// @brief Advances the current token on the parser
static void advance() {
    parser.previous = parser.current;
    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR)
            break;
        errorAtCurrent(parser.current.start);
    }
}

/**
 * @brief Consumes the current token if it matches the given type, otherwise reports an error.
 * @param type The type of token to consume
 * @param message The error message to report if the token does not match the given type
 */
static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    errorAtCurrent(message);
}

/**
 * @brief Emits a byte to the current chunk
 * @param byte The byte to emit
 */
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

/// @brief Finalizes the bytecode for the current chunk being compiled by emitting a return operation.
static void endCompiler() {
    emitByte(OP_RETURN);
}

/// @brief Parses an expression in the source code.
static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

/// @brief Parses a unary expression in the source code.
static void unary() {
    TokenType operatorType = parser.previous.type;

    // Compile the operand.
    parsePrecedence(PREC_UNARY);

    switch (operatorType) {
    case TOKEN_MINUS:
        emitByte(OP_NEGATE);
        break;
    default:
        return;
    }
}

/**
 * @brief Starts at the current token and parses any expression at the given precedence level or higher.
 * @param precedence The precedence to parse
 */
static void parsePrecedence(Precedence precedence) {
    // What goes here?
}

/// @brief Parses a grouping expression ("(" and ")") in the source code.
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

/**
 * @brief Compiles a constant value into the current chunk
 * @param value The value to compile
 * @return The index of the constant in the chunk
 */
static uint8_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);

    if (constant > UINT8_MAX) {
        // Since the OP_CONSTANT instruction uses a single byte for the index operand, we can store and load only up to 256 constants in a chunk.
        // TODO: Implement a solution for this
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}

/**
 * @brief Compiles a constant value into the current chunk
 * @param value The value to compile
 */
static void emitConstant(Value value) {
    emitByte(OP_CONSTANT);
    emitByte(makeConstant(currentChunk()));
    emitByte(value);
}

bool compile(const char* source, Chunk* chunk) {
    initScanner(source);

    compilingChunk = chunk;

    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");

    endCompiler();

    return !parser.hadError;
}
