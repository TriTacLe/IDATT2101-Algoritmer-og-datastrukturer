#include <stdio.h>

int method1(float n, float x) {
  if (n == 0){
    return 0;
  }
  if (n > 1){
    return method1(n-1, x) + x;
  }
  return method1(n+1,x)-x;
}

int main(){
  float result = method1(-3,-4);
  printf("%f",result);
}