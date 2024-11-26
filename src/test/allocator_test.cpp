#include <stdio.h>
#include <cstdint>
#include "allocator/gc_allocator.hpp"


struct test_struct
{
   int first;
   int second;
};


int main()
{
  allocator Al = *allocator::getInstance();
  test_struct* dummy_struct = reinterpret_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  dummy_struct->first = 1;
  dummy_struct->second = 2;
  printf("vaules of struct fields are %d and %d\n", dummy_struct->first, dummy_struct->second);
  Al.free(dummy_struct);
  test_struct* dummy_struct2 = reinterpret_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  dummy_struct2->first = 4;
  dummy_struct2->second = 5;
  printf("vaules of struct fields are %d and %d\n", dummy_struct2->first, dummy_struct2->second);
  Al.free(dummy_struct2);
  return 0;
}
