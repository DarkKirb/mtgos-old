#pragma once
#include "../output/serial.hpp"
#include "../output/display.hpp"
namespace drivers {
namespace serial {
class DisplaySerial: public MTGos::Serial {
public:
    DisplaySerial(multiboot_info_t*);
    ~DisplaySerial();
    virtual auto putChar(char c) -> void;

};
}
}
