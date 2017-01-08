#pragma once
#include <multiboot.h>
namespace drivers {
namespace display {
class VESA_Framebuffer: public MTGos::Display {
public:
    VESA_Framebuffer(multiboot_info_t* mb_info);
    virtual ~VESA_Framebuffer();
    virtual auto plot(int x, int y, uint32_t color) -> void;
    virtual auto getPixel(int x, int y) -> uint32_t;
protected:
    virtual auto destAddr(int x, int y) -> uint8_t*;
};
}
}
