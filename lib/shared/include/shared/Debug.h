#pragma once

#include <shared/Chunk.h>

/**
 * @brief Disassembles the given chunk and prints it to stdout.
 * @param chunk The chunk to disassemble.
 * @param name The name of the chunk.
 */
void disassembleChunk(Chunk* chunk, const char* name);

/**
 * @brief Disassembles the given instruction and prints it to stdout.
 * @param chunk The chunk to disassemble.
 * @param offset The offset of the instruction to disassemble.
 * @return The offset of the next instruction.
 */
int disassembleInstruction(Chunk* chunk, int offset);