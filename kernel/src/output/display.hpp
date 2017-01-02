#include <stdint.h>
#include <multiboot.h>
namespace MTGos {

class Display {
public:
    Display(multiboot_info_t* mb_info);
    virtual ~Display();
    virtual auto putChar(char) -> void = 0;
    auto clearscr() -> void;
    auto scroll() -> void;
    auto render() -> void;
    virtual auto plot(int x, int y, uint32_t color) -> void = 0;
protected:
    multiboot_info_t* mb_info;
    virtual auto destAddr(int x, int y) -> uint32_t* = 0;
    char screenBuf[SCREEN_WIDTH][SCREEN_HEIGHT];
}

};
