#include <interrupts.h>
#include <stdint.h>
namespace MTGos {
const auto Interrupt::syscall(unsigned int callnum, void * handle, void *args) -> unsigned int {
    return 0;
}
}
