#include <stdio.h>
#include <cstdint>
#include "allocator/gc_allocator.hpp"
/*
Synopsis:
Created long chain of memory references on the heap with single reference entry point on the stack
Expected:
Memory not garbage collected
Reason:
Forces GC to recursively scan heap for long reference chains
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
  test_struct* dummy_struct;
  test_struct * tmp = nullptr;
  for(int i = 0; i < 10; i++)
  {
    dummy_struct = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
    dummy_struct->second = tmp;
    tmp = dummy_struct;
  }
  Al.collect();

  return 0;
}
