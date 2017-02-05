#include "../src/output/display.hpp"
#include "vesa_fb.hpp"
namespace drivers {
namespace display {
VESA_Framebuffer::VESA_Framebuffer(multiboot_info_t* mb_info): MTGos::Display(mb_info) {}
VESA_Framebuffer::~VESA_Framebuffer() {}
auto VESA_Framebuffer::plot(int x, int y, uint32_t color) -> void {
    //This is the ideal implementation. On 3DS you'd want to do a __builtin_bswap32(color)>>8
    uint8_t *fbaddr=destAddr(x,y);
    fbaddr[0]=(uint8_t)color;
    fbaddr[1]=(uint8_t)(color>>8);
    fbaddr[2]=(uint8_t)(color>>16);
}
auto VESA_Framebuffer::getPixel(int x, int y) -> uint32_t {
    //This is the ideal implementation. On 3DS you'd want to do a __builtin_bswap32((*destAddr(x,y))<<8)
    uint8_t *fbaddr=destAddr(x,y);
    uint32_t color = fbaddr[0];
    color |= fbaddr[1]<<8;
    color |= fbaddr[2]<<16;
    return color;
}
auto VESA_Framebuffer::destAddr(int x, int y) -> uint8_t * {
    //This is the ideal implementation. On 3DS you'd use y as x and x as y.
    uintptr_t start=mb_info->framebuffer_addr;
    return (uint8_t*)(start+(mb_info->framebuffer_bpp/8)*(x+(y*mb_info->framebuffer_width)));
}
}
}
