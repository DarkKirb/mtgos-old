#pragma once
namespace drivers {
namespace display {
class YUY2Framebuffer: public MTGos::Display {
public:
    YUY2Framebuffer(multiboot_info_t* mb_info);
    virtual ~YUY2Framebuffer();
    virtual auto plot(int x, int y, uint32_t color) -> void;
    virtual auto getPixel(int x, int y) -> uint32_t;
protected:
    virtual auto destAddr(int x, int y) -> uint8_t*;
};
}
}
