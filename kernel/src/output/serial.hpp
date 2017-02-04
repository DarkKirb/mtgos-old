#pragma once
#include <stdint.h>
#include <multiboot.h>
namespace MTGos {
enum class LogLevel: int {
    DEBUG=0,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};
class Serial {
public:
    LogLevel level;
    LogLevel lvl;
    Serial(multiboot_info_t*);
    virtual ~Serial();
    virtual auto putChar(char) -> void = 0;
    virtual auto puts(const char*) -> void;
    template <typename T>
    auto operator<<(T thing) -> Serial & {
        if((int)lvl < (int)level) {
            return *this;
        }
        puts(thing);
        return *this;
    }
protected:
    multiboot_info_t* mb_info;
};
template <>
auto Serial::operator<<<uint64_t>(uint64_t thing) -> Serial &;
template <>
auto Serial::operator<<<LogLevel>(LogLevel thing) -> Serial &;
}
#include "../drivers/serial_null.hpp"
#include "../drivers/serial_to_display.hpp"
#if defined(X86_64) || defined(I686)
#include "../../x86/8250_serial.hpp"
#endif
#ifdef KPRINT_NONE
#define SERIAL drivers::serial::NullSerial
#else
#ifdef KPRINT_SERIAL
#define SERIAL drivers::serial::PC_Serial
#else
#ifdef KPRINT_DISPLAY
#define SERIAL drivers::serial::DisplaySerial
#endif
#endif
#endif
