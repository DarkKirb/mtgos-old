#include "../src/output/display.hpp"
#include "lfb.hpp"
namespace drivers {
namespace display {
LFB::LFB(multiboot_info_t* mb_info): MTGos::Display(mb_info) {}
LFB::~LFB() {}
auto LFB::plot(int x, int y, uint32_t color) -> void {
    //This is the ideal implementation. On 3DS you'd want to do a __builtin_bswap32(color)>>8
    color=__builtin_bswap32(color)>>8;
    uint8_t *fbaddr=destAddr(x,y);
    fbaddr[0]=(uint8_t)color;
    fbaddr[1]=(uint8_t)(color>>8);
    fbaddr[2]=(uint8_t)(color>>16);
}
auto LFB::getPixel(int x, int y) -> uint32_t {
    uint8_t *fbaddr=destAddr(x,y);
    uint32_t color = fbaddr[0];
    color |= fbaddr[1]<<8;
    color |= fbaddr[2]<<16;
    color=__builtin_bswap32(color<<8);
    return color;
}
auto LFB::destAddr(int ox, int oy) -> uint8_t * {
    uintptr_t start=mb_info->framebuffer_addr;
    //xmax,0                       xmax,ymax
    //
    //
    //0,0                          0,ymax
    int x=240-oy-1;
    int y=ox;
    return (uint8_t*)(start+3*(x+(y*240)));
}
}
}
