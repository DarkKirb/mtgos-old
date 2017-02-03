#include "serial.hpp"
namespace MTGos {
Serial::Serial(multiboot_info_t* mb_info): mb_info(mb_info), level(LogLevel::INFO)  {}
Serial::~Serial() {}
template <>
auto Serial::operator<<<uint64_t>(uint64_t i) -> Serial & {
    if((int)lvl < (int)level) {
        return *this;
    }

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
template<>
auto Serial::operator<<<LogLevel>(LogLevel i) -> Serial & {
    lvl=i;
    if((int)lvl < (int)level) {
        return *this;
    }
    switch(lvl) {
        case LogLevel::DEBUG:
            puts("DEBUG:");
            break;
        case LogLevel::INFO:
            puts("INFO:");
            break;
        case LogLevel::WARNING:
            puts("WARNING:");
            break;
        case LogLevel::ERROR:
            puts("ERROR!");
            break;
        case LogLevel::CRITICAL:
            puts("CRITICAL!!!!");
            break;
    }
    return *this;
}
auto Serial::puts(const char * str) -> void {
    for(int i=0;str[i];i++) {
        putChar(str[i]);
    }
}

}
