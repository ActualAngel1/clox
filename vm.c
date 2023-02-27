#include <stdio.h>

#include "common.h"
#include "vm.h"

VM vm;

static void resetStack() {
  vm.stackTop = vm.stack;
}

void initVM() {
  resetStack();
}

void freeVM() {
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

  for (;;) {
      #ifdef DEBUG_TRACE_EXECUTION
              printf("          ");
        for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
          printf("[ ");
          printValue(*slot);
          printf(" ]");
        }
        printf("\n");
         int newArr[vm.chunk.count];
         int temp = vm.chunk.lines;
         for(int m = 1 ; m < chunk.count+1; m ++ ){
              newArr[m-1] = getLine(m, chunk.lines);
        }
        vm.chunk.lines = newArr;
      disassembleInstruction(vm.chunk,
                             (int)(vm.ip - vm.chunk->code));
        vm.chunk.lines = temp;
      #endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        break;
      }
      case OP_ADD:      BINARY_OP(+); break;
      case OP_SUBTRACT: BINARY_OP(-); break;
      case OP_MULTIPLY: BINARY_OP(*); break;
      case OP_DIVIDE:   BINARY_OP(/); break;
      case OP_NEGATE:   push(-pop()); break;
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}