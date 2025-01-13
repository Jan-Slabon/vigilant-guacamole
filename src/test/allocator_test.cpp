#include <stdio.h>
#include <cstdint>
#include "allocator/gc_allocator.hpp"


struct test_struct
{
   int first;
   test_struct* second;
   int arr[100];
};


int main()
{
  allocator Al = allocator::getInstance();
  test_struct* dummy_struct;
  test_struct * tmp = nullptr;
  for(int i = 0; i < 10; i++)
  {
    printf("%d'th iteration\n", i);
    dummy_struct = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
    dummy_struct->first = 1;
    dummy_struct->second = tmp;
    tmp = dummy_struct;
  }
  Al.collect();

  int* dummy_struct2 = static_cast<int *>(Al.reserve(sizeof(int)));
  *dummy_struct2 = 2;
  printf("vaules of struct fields are %d\n", *dummy_struct2);
  Al.free(dummy_struct2);
  test_struct * dummy_struct3;
  dummy_struct3 = static_cast<test_struct *>(Al.reserve(sizeof(test_struct)));
  dummy_struct3->first = 1;
  dummy_struct3->second = dummy_struct;
  printf("vaules of struct fields are %d and %p\n", dummy_struct3->first, dummy_struct3->second);
  Al.free(dummy_struct3);
  return 0;
}
