#pragma once
namespace drivers {
namespace serial {
class PC_Serial: public MTGos::Serial {
public:
    PC_Serial(multiboot_info_t*);
    virtual ~PC_Serial();
    virtual auto putChar(char) -> void;
private:
    bool isTransmitEmpty();
    
};
}
}
