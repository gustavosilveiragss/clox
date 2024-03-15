#include <shared/common.h>
#include <shared/Compiler.h>
#include <shared/Scanner.h>
#include <shared/VM.h>

#ifdef DEBUG_PRINT_CODE
    #include <shared/Debug.h>
#endif

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

/// @brief Emits an OP_RETURN instruction to the current chunk
static void emitReturn() {
    emitByte(OP_RETURN);
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
    emitByte(makeConstant(value));
}

/// @brief Finalizes the bytecode for the current chunk being compiled
static void endCompiler() {
    emitReturn();

#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
#endif
}

/// @brief Parses a number in the source code.
static void expression();

/**
 * @brief Given a token type, returns the corresponding parse rule
 * @param type The token type
 * @return The parse rule for the given token type
 */
static ParseRule* getRule(TokenType type);

/**
 * @brief Starts at the current token and parses any expression at the given precedence level or higher.
 * @param precedence The precedence to parse
 */
static void parsePrecedence(Precedence precedence);

/// @brief Parses a binary expression in the source code.
static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
    case TOKEN_BANG_EQUAL:
        emitByte(OP_EQUAL);
        emitByte(OP_NOT);
        break;
    case TOKEN_EQUAL_EQUAL:
        emitByte(OP_EQUAL);
        break;
    case TOKEN_GREATER:
        emitByte(OP_GREATER);
        break;
    case TOKEN_GREATER_EQUAL:
        emitByte(OP_LESS);
        emitByte(OP_NOT);
        break;
    case TOKEN_LESS:
        emitByte(OP_LESS);
        break;
    case TOKEN_LESS_EQUAL:
        emitByte(OP_GREATER);
        emitByte(OP_NOT);
        break;
    case TOKEN_PLUS:
        emitByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emitByte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        emitByte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        emitByte(OP_DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}

/// @brief Parses a literal value in the source code.
static void literal() {
    switch (parser.previous.type) {
    case TOKEN_FALSE:
        emitByte(OP_FALSE);
        break;
    case TOKEN_NIL:
        emitByte(OP_NIL);
        break;
    case TOKEN_TRUE:
        emitByte(OP_TRUE);
        break;
    default:
        return;
    }
}

/// @brief Parses a number in the source code.
static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

/// @brief Parses a unary expression in the source code.
static void unary() {
    TokenType operatorType = parser.previous.type;

    // Compile the operand.
    parsePrecedence(PREC_UNARY);

    switch (operatorType) {
    case TOKEN_BANG:
        emitByte(OP_NOT);
        break;
    case TOKEN_MINUS:
        emitByte(OP_NEGATE);
        break;
    default:
        return;
    }
}

static void parsePrecedence(Precedence precedence) {
    advance();

    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

/// @brief Parses a grouping expression ("(" and ")") in the source code.
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

/// @brief Map of rules for parsing tokens (Prefix, Infix, Precedence)
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, NULL,   PREC_NONE    },
    [TOKEN_RIGHT_PAREN] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_LEFT_BRACE] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_RIGHT_BRACE] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_COMMA] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_DOT] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_MINUS] = { unary,   binary, PREC_TERM    },
    [TOKEN_PLUS] = { NULL,    binary, PREC_TERM    },
    [TOKEN_SEMICOLON] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_SLASH] = { NULL,    binary, PREC_FACTOR  },
    [TOKEN_STAR] = { NULL,    binary, PREC_FACTOR  },
    [TOKEN_BANG] = { unary,   NULL,   PREC_NONE    },
    [TOKEN_BANG_EQUAL] = { NULL,    binary, PREC_EQUALITY},
    [TOKEN_EQUAL] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_EQUAL_EQUAL] = { NULL,    binary, PREC_EQUALITY},
    [TOKEN_GREATER] = { NULL,    binary, PREC_EQUALITY},
    [TOKEN_GREATER_EQUAL] = { NULL,    binary, PREC_EQUALITY},
    [TOKEN_LESS] = { NULL,    binary, PREC_EQUALITY},
    [TOKEN_LESS_EQUAL] = { NULL,    binary, PREC_EQUALITY},
    [TOKEN_IDENTIFIER] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_STRING] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_NUMBER] = { number,  NULL,   PREC_NONE    },
    [TOKEN_AND] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_CLASS] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_ELSE] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_FALSE] = { literal, NULL,   PREC_NONE    },
    [TOKEN_FOR] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_FUN] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_IF] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_NIL] = { literal, NULL,   PREC_NONE    },
    [TOKEN_OR] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_PRINT] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_RETURN] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_SUPER] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_THIS] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_TRUE] = { literal, NULL,   PREC_NONE    },
    [TOKEN_VAR] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_WHILE] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_ERROR] = { NULL,    NULL,   PREC_NONE    },
    [TOKEN_EOF] = { NULL,    NULL,   PREC_NONE    },
};

static ParseRule* getRule(TokenType type) {
    return &rules[type];
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
