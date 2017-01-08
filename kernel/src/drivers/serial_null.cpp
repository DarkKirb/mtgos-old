#include "../output/serial.hpp"
#include "serial_null.hpp"
namespace drivers {
namespace serial {
NullSerial::NullSerial(multiboot_info_t* mb_info): MTGos::Serial(mb_info) {

}
NullSerial::~NullSerial() {}
auto NullSerial::putChar(char c) -> void {}
}
}
