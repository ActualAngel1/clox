#ifndef clox_decompress_h
#define clox_decompress_h

#include "debug.h"

#define currentLine getLine(offset+1, chunk->lines)
#define nextLine (getLine(offset + 2, chunk->lines))
#define prevLine (getLine(offset, chunk->lines))

int getLine(int offset, int* lines);
void restoreArray(int* old, int* new, Chunk* chunk);
#endif
