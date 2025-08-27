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

double timing(void)
{
  struct timespec time_spec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &time_spec);
  return (double)time_spec.tv_sec + (double)time_spec.tv_nsec * 1e-9;
}

void time_tracking(FunctionPointer methodx)
{
  int n_values[] = {10, 100, 1000, 10000, 100000};
  int length_n_values = sizeof n_values / sizeof n_values[0];
  for (int i = 0; i < length_n_values; i++)
  {
    int n = n_values[i];
    float x = 10;
    if (methodx == method1 && n > 50000)
    {
      // Skipping huge number for method1 to avoid segmentation fault (because its linear), you get stack overflow
      continue;
    }
    int repetitions = (methodx == method2) ? 1000000 : 5;
    float result;
    double start = timing();
    for (int j = 0; j < repetitions; j++)
    {
      result = methodx(n, x);
    }
    double end = timing();
    double time_spent = (end - start) / repetitions;
    printf("n = %d, x = %.1f, result: %.1f, snitt: %.15fs (%.0fns)\n",
           n, x, result, time_spent, time_spent * 1e9);
  }
}

void method_testing(FunctionPointer methodx) {}

int main(void)
{
  printf("Timing method1:\n");
  time_tracking(method1);
  printf("Timing method2:\n");
  time_tracking(method2);
}