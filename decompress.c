#include "chunk.h"

static int getIndex(int offset, const int* const lines){
        int i = 1;
        int sum = lines[i];
        while(sum < offset){
          i+=2;
          sum+=lines[i];
        }
        return i;

}
int getLine(int offset, const int* const lines){
  return lines[getIndex(offset,lines)-1];
}
