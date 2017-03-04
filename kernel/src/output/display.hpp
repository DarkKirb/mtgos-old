#pragma once
#include <stdint.h>
#include <multiboot.h>
#include <defines.h>
namespace MTGos {

class Display {
public:
    Display(multiboot_info_t* mb_info);
    virtual ~Display();
    auto putChar(int) -> void;
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
    int buffer[SCREEN_WIDTH][SCREEN_HEIGHT];
    multiboot_info_t* mb_info;
    virtual auto destAddr(int x, int y) -> uint8_t* = 0;
    int x,y;
private:
    auto plotChar(int x, int y, int c) -> void;
};
template <>
auto Display::operator<<<uint64_t>(uint64_t thing) -> Display &;
}
#include "../drivers/display_null.hpp"
#if defined(X86_64) || defined(I686)
#include "../../x86/vesa_fb.hpp"
#elif defined(_3DS9) || defined(_3DS11)
#include "../../3ds/lfb.hpp"
#elif defined(WII)
#include "../../wii/yuy2lfb.hpp"
#endif
#ifdef DISPLAY_NONE
#define DISPLAY drivers::display::NullDisplay
#else
#ifdef DISPLAY_VESA
#define DISPLAY drivers::display::VESA_Framebuffer
#else
#ifdef DISPLAY_LFB
#define DISPLAY drivers::display::LFB
#else
#ifdef DISPLAY_YUY2LFB
#define DISPLAY drivers::display::YUY2Framebuffer
#endif
#endif
#endif
#endif
