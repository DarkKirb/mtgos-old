#include <stdint.h>
#include <base.hpp>
extern "C" void __cxa_pure_virtual() {
    //TODO Insert panic here
}
void * operator new(size_t s) {
    return pmm.alloc(s);
}
void * operator new[](size_t s) {
    return pmm.alloc(s);
}
void operator delete(void* p) {
    pmm.free(p,1);
};
void operator delete[](void* p) {
    pmm.free(p,1);
};
void operator delete(void* p, size_t s) {
    pmm.free(p,s);
};
void operator delete[](void* p, size_t s) {
    pmm.free(p,s);
};
void * operator new (size_t s, void * p)  {
    pmm.markUsed(p);
    return p;
}
void * operator new[] (size_t, void * p)  {
    pmm.markUsed(p);
    return p;
}
void operator delete (void *, void * p)  {
    pmm.free(p,1);
}
void operator delete[] (void *, void *p) {
    pmm.free(p,1);
}
#if __x86_64__ || __ppc64__
#define STACK_CHK_GUARD BUILDID
#else
#define STACK_CHK_GUARD ((uintptr_t)BUILDID)
#endif
uintptr_t __stack_chk_guard_val = STACK_CHK_GUARD;
uintptr_t __stack_chk_guard = (uintptr_t)(&__stack_chk_guard_val);
extern "C" __attribute__((noreturn))
void __stack_chk_fail() {
    panic("Stack guard failed check.");
}
//And because we use PIC:
extern "C" __attribute__ ((visibility ("hidden")))
void __stack_chk_fail_local()
{
  __stack_chk_fail();
}
