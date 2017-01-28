#pragma once
#include <stdint.h>
#include <multiboot.h>
namespace MTGos {

class Display {
public:
    Display(multiboot_info_t* mb_info);
    virtual ~Display();
    auto putChar(char) -> void;
    auto clearscr() -> void;
    auto scroll() -> void;
    virtual auto puts(const char*) -> void;
    template <typename T>
    auto operator << (T thing) -> Display & {
        puts(thing);
        return *this;
    }
    virtual auto plot(int x, int y, uint32_t color) -> void = 0;
    virtual auto getPixel(int x, int y) -> uint32_t =0;
protected:
    multiboot_info_t* mb_info;
    virtual auto destAddr(int x, int y) -> uint8_t* = 0;
    int x,y;
};
template <>
auto Display::operator<<<uint64_t>(uint64_t thing) -> Display &;
}

#ifdef DISPLAY_NONE
#include "../drivers/display_null.hpp"
#define DISPLAY drivers::display::NullDisplay
#else
#ifdef DISPLAY_VESA
#include "../../x86/vesa_fb.hpp"
#define DISPLAY drivers::display::VESA_Framebuffer
#else
#ifdef DISPLAY_LFB
#include "../../3ds/lfb.hpp"
#define DISPLAY drivers::display::LFB
#endif
#endif
#endif
