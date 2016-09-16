#pragma once
namespace MTGos {
class Interrupt {
private:
public:
    virtual auto registerHandler() -> void {};
    const auto syscall(unsigned int callnum, void * handle, void *args) -> unsigned int;
    Interrupt() {};
    virtual ~Interrupt() {};
};
}
