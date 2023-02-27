#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "decompress.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 123);
  writeChunk(&chunk, OP_RETURN, 1);
  writeChunk(&chunk, OP_RETURN, 1);
  writeChunk(&chunk, OP_RETURN, 2);
  writeChunk(&chunk, OP_RETURN, 3);
  writeChunk(&chunk, OP_RETURN, 4);
  writeChunk(&chunk, OP_RETURN, 5);

  int newArr[chunk.count];

  for(int m = 1 ; m < chunk.count+1; m ++ ){
    newArr[m-1] = getLine(m, chunk.lines);
  }

  chunk.lines = newArr;
  disassembleChunk(&chunk, "test chunk");
  interpret(&chunk);
  freeVM();
  freeChunk(&chunk);
  return 0;
}