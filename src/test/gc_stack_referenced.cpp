#include <stdio.h>
#include <cstdint>
#include "allocator/gc_allocator.hpp"
/*
Synopsis:
Created array of references to heap allocated data
Expected:
Memory not garbage collected
Reason:
Data referenced on stack might be still used
*/

struct test_struct
{
   int first;
   test_struct* second;
   int arr[100];
};

using Allocator = gc::allocator;

int main()
{
  Allocator Al = Allocator::getInstance();
  test_struct* dummy_struct[10];
  for(int i = 0; i < 10; i++)
  {
    dummy_struct[i] = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  }
  Al.collect();

  return 0;
}
