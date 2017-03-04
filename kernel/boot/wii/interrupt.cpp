#include "../src/base.hpp"
extern "C" void panic(char* c ) {
    out << "PANIC! " << c;
    for(;;);
}
