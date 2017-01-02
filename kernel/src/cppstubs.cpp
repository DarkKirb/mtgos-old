#include <stdint.h>
extern "C" void __cxa_pure_virtual() {
    //TODO Insert panic here
}
void * operator new(size_t) {
    return (void*)1u; //TODO add panic()
}
void * operator new[](size_t) {
    return (void*)1u; //TODO add panic()
}
void operator delete(void*) {};
void operator delete[](void*) {};
void operator delete(void*, size_t) {};
void operator delete[](void*, size_t) {};
void * operator new (size_t, void * p)  { return p ; }
void * operator new[] (size_t, void * p)  { return p ; }
void operator delete (void *, void *)  { }
void operator delete[] (void *, void *) { }
#if __x86_64__ || __ppc64__
#define STACK_CHK_GUARD BUILDID
#else
#define STACK_CHK_GUARD ((uintptr_t)BUILDID)
#endif
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
extern "C" __attribute__((noreturn))
void __stack_chk_fail() {
    for(;;); //TODO panic
}
//And because we use PIC: 
extern "C" __attribute__ ((visibility ("hidden")))
void __stack_chk_fail_local()
{
  __stack_chk_fail();
}
