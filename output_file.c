#include <stdio.h>

#include "debug.h"
#include "value.h"
#include "object.h"

FILE *out_file = NULL;

void writeValue(Value value);

void writeToFile(Chunk* chunk, const char* name) {
  out_file = fopen("bytecode.txt", "a");
  if (out_file == NULL) fprintf(stderr, "File fail");

  fprintf(out_file, "\n");
  fprintf(out_file, "function %s \n", name);
  fprintf(out_file, "----------\n");

  // TODO: Decompress the line buffer
  fprintf(out_file, "lines: \n");
  for (int i = 0; i < sizeof(chunk->lines)/sizeof(chunk->lines[0]); i++) {
    fprintf(out_file, "%d \n", chunk->lines[i]);
  }

  fprintf(out_file, "constants: \n");
  for (int i = 0; i < chunk->constants.count; i++) {
    Value value = chunk->constants.values[i];
    writeValue(value);
    fprintf(out_file, "\n");
  }

  fprintf(out_file, "instructions: \n");
  for (int offset = 0; offset < chunk->count;) {
    offset = writeInstruction(chunk, offset);
  }

  fclose(out_file);
}

static void writeFunction(ObjFunction* function) {
  if (function->name == NULL) {
    fprintf(out_file, "<script>");
    return;
  }
  fprintf(out_file, "<fn %s> \n", function->name->chars);
}

void writeObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_FUNCTION:
      writeFunction(AS_FUNCTION(value));
      break;
    case OBJ_NATIVE:
      fprintf(out_file, "<native fn>");
      break;
    case OBJ_STRING:
      fprintf(out_file, "%s", AS_CSTRING(value));
      break;
  }
}

void writeValue(Value value) {
    switch (value.type) {
    case VAL_BOOL:
      fprintf(out_file, AS_BOOL(value) ? "true" : "false");
      break;
    case VAL_NIL: fprintf(out_file, "nil"); break;
    case VAL_NUMBER: fprintf(out_file, "%g", AS_NUMBER(value)); break;
    case VAL_OBJ: writeObject(value); break;
  }
}

static int constantInstruction(const char* name, Chunk* chunk,
                               int offset) {
  uint8_t constant = chunk->code[offset + 1];
  fprintf(out_file, "%s \n%d \n", name, constant);
  writeValue(chunk->constants.values[constant]);
  fprintf(out_file, "\n");
  return offset + 2;
}

static int simpleInstruction(const char* name, int offset) {
  fprintf(out_file, "%s\n", name);
  return offset + 1;
}

static int byteInstruction(const char* name, Chunk* chunk,
                           int offset) {
  uint8_t slot = chunk->code[offset + 1];
  fprintf(out_file, "%s \n%d \n", name, slot);
  return offset + 2;
}

static int jumpInstruction(const char* name, int sign,
                           Chunk* chunk, int offset) {
  uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
  jump |= chunk->code[offset + 2];
  fprintf("%s \n%d \n to %d\n", name, offset,
         offset + 3 + sign * jump);
  return offset + 3;
}

int writeInstruction(Chunk* chunk, int offset) {
  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_NIL:
      return simpleInstruction("OP_NIL", offset);
    case OP_TRUE:
      return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
      return simpleInstruction("OP_FALSE", offset);
    case OP_POP:
      return simpleInstruction("OP_POP", offset);
    case OP_GET_LOCAL:
      return byteInstruction("OP_GET_LOCAL", chunk, offset);
    case OP_SET_LOCAL:
      return byteInstruction("OP_SET_LOCAL", chunk, offset);
    case OP_GET_GLOBAL:
      return constantInstruction("OP_GET_GLOBAL", chunk, offset);
    case OP_DEFINE_GLOBAL:
      return constantInstruction("OP_DEFINE_GLOBAL", chunk,
                                 offset);
    case OP_SET_GLOBAL:
      return constantInstruction("OP_SET_GLOBAL", chunk, offset);
    case OP_EQUAL:
      return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
      return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
      return simpleInstruction("OP_LESS", offset);
    case OP_ADD:
      return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
      return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
      return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
      return simpleInstruction("OP_DIVIDE", offset);
    case OP_NOT:
      return simpleInstruction("OP_NOT", offset);
    case OP_NEGATE:
      return simpleInstruction("OP_NEGATE", offset);
    case OP_PRINT:
      return simpleInstruction("OP_PRINT", offset);
    case OP_JUMP:
      return jumpInstruction("OP_JUMP", 1, chunk, offset);
    case OP_JUMP_IF_FALSE:
      return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
    case OP_LOOP:
      return jumpInstruction("OP_LOOP", -1, chunk, offset);
    case OP_CALL:
      return byteInstruction("OP_CALL", chunk, offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}







