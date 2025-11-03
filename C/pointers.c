#include <stdio.h>

void birthday(int *age); // function prototype

int main()
{

  // pointer = variable that stores the memory address of another variable
  // * = deference operator
  // & = memory address referencer

  int age = 21;     // holds the integer 21
  int *pAge = &age; // declares a pointer and assign it the address of age

  birthday(pAge);

  printf("You are %d years old", age);

  return 0;
}

void birthday(int *age)
{
  *(age)++;
}