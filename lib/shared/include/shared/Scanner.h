#pragma once

/**
 * @brief Token types, as defined by the Lox language's grammar
 */
typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND,
    TOKEN_CLASS,
    TOKEN_ELSE,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_FUN,
    TOKEN_IF,
    TOKEN_NIL,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VAR,
    TOKEN_WHILE,
    // Error.
    TOKEN_ERROR,
    // End of file.
    TOKEN_EOF
} TokenType;

/**
 * @brief Scanner struct
 * @var Scanner::start The start of the current lexeme.
 * @var Scanner::current The current character.
 * @var Scanner::line The current line of the lexeme.
 */
typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

/**
 * @brief Token struct
 * @var Token::type The type of the token.
 * @var Token::start The start of the lexeme.
 * @var Token::length The length of the lexeme.
 * @var Token::line The line of the lexeme.
 */
typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

/**
 * @brief Initialize the scanner with the source code
 * @param source The source code to scan
 */
void initScanner(const char* source);

/**
 * @brief Scans the token at the current position
 * @return The scanned token
 */
Token scanToken();
