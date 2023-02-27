#ifndef clox_getLine_h
#define clox_getLine_h

#define currentLine chunk->lines[(getLine(offset, chunk->lines)-2)*2]
#define nextLine chunk->lines[(getLine(offset + 1, chunk->lines)-2)*2]
#define prevLine chunk->lines[(getLine(offset - 1, chunk->lines)-2)*2]

int getLine(int offset, int* lines);

#endif
