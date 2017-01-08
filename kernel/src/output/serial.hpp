#pragma once
#include <stdint.h>
#include <multiboot.h>
namespace MTGos {
class Serial {
public:
    Serial(multiboot_info_t*);
    virtual ~Serial();
    virtual auto putChar(char) -> void = 0;
    virtual auto puts(const char*) -> void;
    template <typename T>
    auto operator<<(T thing) -> Serial & {
        puts(thing);
        return *this;
    }
protected:
    multiboot_info_t* mb_info;
};
template <>
auto Serial::operator<<<uint64_t>(uint64_t thing) -> Serial &;
}
#ifdef KPRINT_NONE
#include "../drivers/serial_null.hpp"
#define SERIAL drivers::serial::NullSerial
#else
#ifdef KPRINT_SERIAL
#include "../../i686/8250_serial.hpp"
#define SERIAL drivers::serial::PC_Serial
#endif
#endif
