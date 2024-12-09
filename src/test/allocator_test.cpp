#include <stdio.h>
#include <cstdint>
#include "allocator/gc_allocator.hpp"


struct test_struct
{
   int first;
   int second;
   int arr[100];
};


int main()
{
  allocator Al = *allocator::getInstance();
  test_struct* dummy_struct = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  dummy_struct->first = 1;
  dummy_struct->second = 2;
  printf("vaules of struct fields are %d and %d\n", dummy_struct->first, dummy_struct->second);
  Al.free(dummy_struct);
  int* dummy_struct2 = static_cast<int *>(Al.reserve(sizeof(int)));
  *dummy_struct2 = 2;
  printf("vaules of struct fields are %d\n", *dummy_struct2);
  Al.free(dummy_struct2);
  printf("New case\n");
  dummy_struct = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  dummy_struct->first = 1;
  dummy_struct->second = 2;
  printf("vaules of struct fields are %d and %d\n", dummy_struct->first, dummy_struct->second);
  Al.free(dummy_struct);
  return 0;
}
