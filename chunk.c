#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->lineCount = 0;
  chunk->lineCapacity = 0;
  initValueArray(&chunk->constants);
}

static int growLinesArray(Chunk* chunk, int line){
    if(chunk->lineCount>0 && *((chunk->lines+2*chunk->lineCount-2)) == line){
        chunk->lines[chunk->lineCount*2-1]++;
    }
    else{
      chunk->lines[chunk->lineCount*2]=line;
      chunk->lines[chunk->lineCount*2+1]++;
      chunk->lineCount++;
    }
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code,
        oldCapacity, chunk->capacity);
  }
  if (chunk->lineCapacity < (chunk->lineCount + 1)*2) {
    int oldCapacity = chunk->lineCapacity;
    chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
    chunk->lines = GROW_ARRAY(uint8_t, chunk->lines,
        oldCapacity, chunk->lineCapacity);
  }

  chunk->code[chunk->count] = byte;
  growLinesArray(chunk, line);
  chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}
