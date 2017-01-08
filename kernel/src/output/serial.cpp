#include "serial.hpp"
namespace MTGos {
Serial::Serial(multiboot_info_t* mb_info): mb_info(mb_info) {}
Serial::~Serial() {}
template <>
auto Serial::operator<<<uint64_t>(uint64_t i) -> Serial & {
    //base is 16
    uint64_t output=i;
    const char* chars="0123456789ABCDEF";
    char buf[65];
    buf[64]='\0';
    char* ptr=buf+63;
    do {
        *(ptr--)=chars[output&0xF];
        output>>=4;
    } while(output && (ptr!=buf));
    puts(ptr+1);
    return *this;
}
auto Serial::puts(const char * str) -> void {
    for(int i=0;str[i];i++) {
        putChar(str[i]);
    }
}

}
