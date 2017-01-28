#pragma once
#include <multiboot.h>
namespace drivers {
namespace display {
class LFB: public MTGos::Display {
public:
    LFB(multiboot_info_t* mb_info);
    virtual ~LFB();
    virtual auto plot(int x, int y, uint32_t color) -> void;
    virtual auto getPixel(int x, int y) -> uint32_t;
protected:
    virtual auto destAddr(int x, int y) -> uint8_t*;
};
}
}
