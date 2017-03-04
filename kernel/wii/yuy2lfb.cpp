#include "../src/output/display.hpp"
#include "yuy2lfb.hpp"
//Source: http://stackoverflow.com/a/14697130
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

// YUV -> RGB
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )

#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

// RGB -> YCbCr
#define CRGB2Y(R, G, B) CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16)
#define CRGB2Cb(R, G, B) CLIP((36962 * (B - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)
#define CRGB2Cr(R, G, B) CLIP((46727 * (R - CLIP((19595 * R + 38470 * G + 7471 * B ) >> 16) ) >> 16) + 128)

// YCbCr -> RGB
#define CYCbCr2R(Y, Cb, Cr) CLIP( Y + ( 91881 * Cr >> 16 ) - 179 )
#define CYCbCr2G(Y, Cb, Cr) CLIP( Y - (( 22544 * Cb + 46793 * Cr ) >> 16) + 135)
#define CYCbCr2B(Y, Cb, Cr) CLIP( Y + (116129 * Cb >> 16 ) - 226 )

namespace drivers {
namespace display {
YUY2Framebuffer::YUY2Framebuffer(multiboot_info_t* mb_info): MTGos::Display(mb_info) {}
YUY2Framebuffer::~YUY2Framebuffer() {}
auto YUY2Framebuffer::plot(int x, int y, uint32_t color) -> void {
    //Converting color to YUV
/*
    uint8_t r=(color>>16)&0xFF;
    uint8_t g=(color>>8)&0xFF;
    uint8_t b=color&0xFF;

    uint8_t ay=RGB2Y(r,g,b);
    uint8_t o=0;
    bool uorv = x%2;
    if(!uorv) {
        o=RGB2U(r,g,b);
    } else {
        o=RGB2V(r,g,b);
    }
*/
    uint32_t *dest=(uint32_t*)destAddr(x,y);
    dest[0]=color;
/*    if(!uorv) {
        dest[0]|=(ay<<24)+(o<<16);
    } else {
        dest[1]|=(ay<<8)+o;
    }
*/
}
auto YUY2Framebuffer::getPixel(int x, int y) -> uint32_t {
    //I bet this will fail in spectacular ways in the future because there is literally data missing
    //bool uorv = x%2;
    uint32_t *dest=(uint32_t*)destAddr(x,y);
    return *dest;
    /*uint8_t ay,u,v;
    if(!uorv) {
        ay=(uint8_t)(dest[0]>>24);
        u=(uint8_t)(dest[0]>>16);
        v=0;
    } else {
        ay=(uint8_t)(dest[0]>>8);
        u=0;
        v=(uint8_t)(dest[0]);
    }

    uint8_t r=YUV2R(ay,u,v);
    uint8_t g=YUV2G(ay,u,v);
    uint8_t b=YUV2B(ay,u,v);
    return (((uint32_t)r)<<16) + (((uint32_t)g)<<8) + b;*/
}
auto YUY2Framebuffer::destAddr(int x, int y) -> uint8_t* {
    x+=32; //overscan
    y+=32;
    for(int x=0;x<10000;x++) {
        ((uint8_t*)mb_info->framebuffer_pitch)[x]=0xAFFE;
    }
    return (uint8_t*)(mb_info->framebuffer_pitch+2*(x+(y*mb_info->framebuffer_width)));
}
}
}
