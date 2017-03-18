#include "../src/output/serial.hpp"
#include "../src/output/display.hpp"
#include "../src/pmm.hpp"
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
