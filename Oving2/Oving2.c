#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// n is positive interger, x is float
float method1(int n, float x)
{
  // printf("n: %d, x: %f", n, x);
  if (n == 0)
    return 0;
  if (n == 1)
    return x;
  if (n > 1)
  {
    /*
    float result = method1(n-1, x) + x;
    printf("%f",result);
    */
    return method1(n - 1, x) + x;
  }
  // If n is negative (not the case in the task)
  /*
  float result = method1(n+1,x)-x;
  printf("%f",result);
  */
  return method1(n + 1, x) - x;
}

float method2(int n, float x)
{
  // printf("n: %d, x: %f", n, x);
  if (n == 0)
    return 0;
  if (n == 1)
  {
    return x;
  }
  // Partall
  if (n % 2 == 0)
  {
    /*
    float result = method1(n/2,x+x);
    printf("%f",result);
    */
    return method2(n / 2, x + x);
  }
  // Oddetall
  /*
  float result = x + (method1(n-1/2,x+x));
  printf("%f",result);
  */
  return x + (method2(((n - 1) / 2), x + x));
}

float method1(int number, float value);
float method2(int number, float value);
typedef float (*FunctionPointer)(int, float);

void time_tracking(FunctionPointer methodx)
{
  int n_values[] = {10, 100, 1000, 10000, 100000, 1000000};
  int length_n_values = sizeof n_values / sizeof n_values[0];
  for (int i = 0; i < length_n_values - 1; i++)
  {
    int n = n_values[i];
    float x = 10;
    clock_t begin = clock();
    float m2 = methodx(n, x);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("n = %d, x = %f, n * x: %f, tidsmåling: %f\n", n, x, m2, time_spent);
  }
}

int main(void)
{
  printf("Timing method1:\n");
  time_tracking(method1);
  printf("Timing method2:\n");
  time_tracking(method2);
}