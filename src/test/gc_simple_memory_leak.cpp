#include <stdio.h>
#include <cstdint>
#include "allocator/gc_allocator.hpp"
/*
Synopsis:
Simple memory leak scenario
Expected:
Memory garbage collected
Reason:
Ensures that GC will collect garbage when triggered 
*/

struct test_struct
{
   int first;
   int arr[100];
};

using Allocator = gc::allocator;

int main()
{
  Allocator Al = Allocator::getInstance();
  test_struct* dummy_struct;
  for(int i = 0; i < 10; i++)
  {
    dummy_struct = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  }
  dummy_struct = nullptr;
  Al.collect();

  return 0;
}
