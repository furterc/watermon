#include <stdlib.h>

#include "cplus_plus.h"

void * operator new(size_t size, unsigned int i)
{
  return malloc(size);
}

void operator delete(void * ptr, unsigned int i)
{
  free(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};
void __cxa_pure_virtual() { while (1); }
