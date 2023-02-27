int getLine(int offset, int* lines){
  offset++;
  int sum = 0;
  int* copy = lines;
  copy++;
  int i = 1;
  while(sum < offset){
      sum+=*copy;
      copy+=2;
      i++;
  }
  return i;
}
