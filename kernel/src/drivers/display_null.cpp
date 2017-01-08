#include "../output/display.hpp"
#include "display_null.hpp"
namespace drivers {
namespace display {
NullDisplay::NullDisplay(multiboot_info_t* mb_info): MTGos::Display(mb_info) {}
NullDisplay::~NullDisplay() {}
auto NullDisplay::plot(int x, int y, uint32_t color) -> void {}
auto NullDisplay::getPixel(int x, int y) -> uint32_t {return 0;}
auto NullDisplay::destAddr(int x, int y) -> uint8_t* {return nullptr;}
}
}
