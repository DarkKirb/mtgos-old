#include "../output/serial.hpp"
#include "serial_to_display.hpp"
#include "../base.hpp"
namespace drivers {
namespace serial {
DisplaySerial::DisplaySerial(multiboot_info_t* mb_info): MTGos::Serial(mb_info) {

}
DisplaySerial::~DisplaySerial() {}
auto DisplaySerial::putChar(char c) -> void {
out.putChar(c);
}
}
}
