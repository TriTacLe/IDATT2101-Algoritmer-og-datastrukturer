#include <stdio.h>
#include <stdlib.h>

// n is positive interger, x is float 
float method1(int n, float x) {
  if (n == 1){
    return x;
  }
  if (n > 1){
    return method1(n-1, x) + x;
  }
  // If n is negative (not the case in the task)
  return method1(n+1,x)-x;
}

float method2(int n, float x){
  if (n == 1){
    return x;
  }
  // Partall
  if (n%2== 0){
    return method2(n/2,x+x);
  }
  // Oddetall
  return x + (method2(((n-1)/2), x+x));
}

int main(){ 
  // float m1 = method1(-3,4);
  for (int i = 0; i < 10; i++){
    int min = -10;
    int max = 10;

    int n = rand() % 10 + 1;
    float x = (rand() % (max-min + 1) + min);
    float m2 = method2(n,x);
    printf("n = %d, x = %f, n * x: %f\n", n, x, m2)  ;
  }
}