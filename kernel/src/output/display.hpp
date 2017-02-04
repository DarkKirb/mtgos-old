#pragma once
#include <stdint.h>
#include <multiboot.h>
#include <defines.h>
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
    char buffer[SCREEN_WIDTH][SCREEN_HEIGHT];
    multiboot_info_t* mb_info;
    virtual auto destAddr(int x, int y) -> uint8_t* = 0;
    int x,y;
private:
    auto plotChar(int x, int y, char c) -> void;
};
template <>
auto Display::operator<<<uint64_t>(uint64_t thing) -> Display &;
}
#include "../drivers/display_null.hpp"
#if defined(X86_64) || defined(I686)
#include "../../x86/vesa_fb.hpp"
#elif defined(_3DS9) || defined(_3DS11)
#include "../../3ds/lfb.hpp"
#endif
#ifdef DISPLAY_NONE
#define DISPLAY drivers::display::NullDisplay
#else
#ifdef DISPLAY_VESA
#define DISPLAY drivers::display::VESA_Framebuffer
#else
#ifdef DISPLAY_LFB
#define DISPLAY drivers::display::LFB
#endif
#endif
#endif
