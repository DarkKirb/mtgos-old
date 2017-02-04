#include <stdint.h>
#include <cpuid.h>
int sse=1,sse2=1,sse3=0,ssse3=0,sse41=0,sse42=0,avx=0;
void loadIDT();
void platform_init() {
    uint32_t eax, ebx, ecx, edx;
    __get_cpuid(1, &eax, &ebx, &ecx, &edx);
    if(ecx & bit_SSE3)
        sse3=1;
    if(ecx & bit_SSSE3)
        ssse3=1;
    if(ecx & bit_SSE4_1)
        sse41=1;
    if(ecx & bit_SSE4_2)
        sse42=1;
    loadIDT();
}
#define MIN(x,y) ((x)<(y)?(x):(y))
static const void* memclr_sse2(const void* const m_start, const size_t m_count) {
    size_t i = 0;
    if((size_t)m_start & 0xF) {
        i+=MIN(((size_t)m_start+0x10)&0xFFFFFFF0,m_count);
        asm("rep stosb;"::"D"((size_t)m_start), "a"(0), "c"(i));
    }
    asm volatile("pxor %xmm0, %xmm0");
    for(;i+64 < m_count; i+=64) {
        asm volatile("movdqa %%xmm0, 0(%0); \
                      movdqa %%xmm0, 16(%0); \
                      movdqa %%xmm0, 32(%0); \
                      movdqa %%xmm0, 48(%0)"::"r"((size_t)m_start+i));
    }
    asm("rep stosb;"::"a"(0),"D"(((size_t)m_start)+i),"c"(m_count-i));
    i+=m_count-1;
    return (void*)(((size_t)m_start)+i);
}
static const void* memclr_std(const void* const m_start, const size_t m_count) {
    asm("rep stosl;"::"a"(0),"D"((size_t)m_start),"c"(m_count / 4));
    asm("rep stosb;"::"a"(0),"D"(((size_t)m_start) + ((m_count / 4) * 4)),"c"(m_count - ((m_count / 4) * 4)));

    return (void *)((size_t)m_start + m_count);
}
const void * memclr(const void * const mem, const size_t count)
{
    // return if there is no count
    if(!count){ return mem; }
    // if the CPU is SSE2 capable, we favor the SSE2 version of this function first
    else if(sse2)
    {
        return memclr_sse2(mem, count);
    }
    // if the CPU didn't support SSE2, then we use the generic standard function
    return memclr_std(mem, count);
}
static size_t strlen_sse4_2(const char * const str)
{
   size_t index;

   asm(" mov $-16, %0;                      "
       " pxor %%xmm0, %%xmm0;               "
       ".strlen_4_2_start:                  "
       " add $16, %0;                       "
       " pcmpistri $0x08, (%0,%1), %%xmm0;  "
       " jnz .strlen_4_2_start;             "
       " add %2, %0;                        "
       :"=a"(index):"d"((size_t)str),"c"((size_t)str));

    return index;
}
static size_t strlen_std(const char * const str)
{
    size_t tmp;

    tmp = (size_t)str;

    asm(".strlen_start:"
    "    cmpb $0, 0(%1);"
    "	 jz .strlen_done;"
    "    cmpb $0, 1(%1);"
    "	 jz .strlen_1;"
    "    cmpb $0, 2(%1);"
    "	 jz .strlen_2;"
    "    cmpb $0, 3(%1);"
    "	 jz .strlen_3;"
    "    cmpb $0, 4(%1);"
    "	 jz .strlen_4;"
    "    cmpb $0, 5(%1);"
    "	 jz .strlen_5;"
    "    cmpb $0, 6(%1);"
    "	 jz .strlen_6;"
    "    cmpb $0, 7(%1);"
    "	 jz .strlen_7;"
    // if no 0s were found, add 8 and start again
    "	 add $8, %1;"
    "	 jmp .strlen_start;"
    // here are all the expanded functions that get called once a 0 is found
    ".strlen_1:"
    "	 inc %1;"
    "	 jmp .strlen_done;"
    ".strlen_2:"
    "	 add $2, %1;"
    "	 jmp .strlen_done;"
    ".strlen_3:"
    "	 add $3, %1;"
    "	 jmp .strlen_done;"
    ".strlen_4:"
    "	 add $4, %1;"
    "	 jmp .strlen_done;"
    ".strlen_5:"
    "	 add $5, %1;"
    "	 jmp .strlen_done;"
    ".strlen_6:"
    "	 add $6, %1;"
    "	 jmp .strlen_done;"
    ".strlen_7:"
    "	 add $7, %1;"
    // output the current value, and exit.
    ".strlen_done:"
    "	mov %1, %0;"
    :"=r"(tmp):"0"(tmp));

    return tmp - (size_t)str;
}
size_t strlen(const char * const str)
{
    // check for SSE 4.2
    if(sse42)
    {
        return strlen_sse4_2(str);
    }

    return strlen_std(str);
}
