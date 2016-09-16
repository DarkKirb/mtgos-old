#include <moduleinterface.h>
#include <base/output.hpp>
#include <modstubs.h>
int x=0,y=0;
uint16_t* vmem = (uint16_t*)0xB8000;
namespace MTGos {
namespace {
/**
 * \class Screen
 * \brief Text output to the screen
 */
class Screen: public Base::Output {
public:
    Screen() {x=0;y=0;vmem = (uint16_t*)0xB8000;}
    auto scroll() -> void {
        for(int p=0;p<80*25;p++)
            vmem[p]=vmem[p+80];
        for(int p=80*25; p<81*25; p++)
            vmem[p]=0x0700;
        y--;
    }
private:
    virtual auto putChar(int c) -> void{
        switch(c) {
        case '\n':
            x=0; y++;
            break;
        case '\r':
            x=0;
            break;
        case '\b':
            if(x==0)
                break;
            x--;
            putChar(' '),
            x--;
            break;
        case '\0':
            break;
        default:
            vmem[y*80+(x++)]=((uint16_t)0x0700) | ((uint8_t)c);
            if(x==80) {
                x=0; y++;
            }
            break;
        }
        if(y>25)
            scroll();
    }
};
}
}
table_type getTable() {
    void(**tbl)()=(void(**)())0x0;
    tbl[0]=(void(*)())&getType;
    tbl[1]=(void(*)())&size_of;
    tbl[2]=(void(*)())&spawnAt;
    doCtors();
    return (void(**)())0x0;
}
auto getType() -> ModType {
    return ModType::output_text;
}
auto spawnAt(void* pos) -> bool {
    new(pos) MTGos::Screen;
    return true;
}
auto size_of() -> size_t {
    return sizeof(MTGos::Screen);
}
