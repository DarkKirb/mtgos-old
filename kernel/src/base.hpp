#include "output/serial.hpp"
#include "output/display.hpp"
#include "pmm.hpp"
extern DISPLAY out;
extern SERIAL kout;
extern PMMD pmm;
void operator delete(void*);
void operator delete[](void*);
void operator delete(void*, size_t);
void operator delete[](void*, size_t);
void * operator new (size_t, void * p);
void * operator new[] (size_t, void * p);
void operator delete (void *, void *);
void operator delete[] (void *, void *);
extern "C" void panic(char*);
