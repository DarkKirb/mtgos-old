#pragma once
#include "../output/serial.hpp"
namespace drivers {
namespace serial {
class NullSerial: public MTGos::Serial {
public:
    NullSerial(multiboot_info_t*);
    ~NullSerial();
    virtual auto putChar(char c) -> void;

};
}
}
