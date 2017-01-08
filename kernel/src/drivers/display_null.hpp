#pragma once
namespace drivers {
namespace display {
class NullDisplay: public MTGos::Display {
public:
    NullDisplay(multiboot_info_t* mb_info);
    virtual ~NullDisplay();
    virtual auto plot(int x, int y, uint32_t color) -> void;
    virtual auto getPixel(int x, int y) -> uint32_t;
protected:
    virtual auto destAddr(int x, int y) -> uint8_t*;
};
}
}
