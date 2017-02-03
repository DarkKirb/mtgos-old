#include "display.hpp"
#include "font.hpp"
#include <defines.h>
namespace MTGos {

Display::Display(multiboot_info_t* mb_info): mb_info(mb_info), x(0),y(0) {
    for(int x=0;x<SCREEN_WIDTH;x++)
        for(int y=0;y<SCREEN_HEIGHT;y++)
            buffer[x][y]=0;
}
Display::~Display() {}
auto Display::putChar(char c) -> void {
    if(c<0) {
        return;
    }
    switch(c) {
        case '\n':
            x=0;
            y++;
            if(y>=SCREEN_HEIGHT) {
                scroll();
            }
            return;
    }
    buffer[x][y]=c;
    for(int x=0;x<8;x++) {
        for(int y=0;y<8;y++) {
            plot(this->x*8+x,this->y*8+y,(font[c][y]&(1<<x))?0xFFFFFF:0);
        }
    }
    x++;
    if(x>=SCREEN_WIDTH) {
        x=0;
        y++;
    }
    if(y>=SCREEN_HEIGHT) {
        scroll();
    }
}
auto Display::clearscr() -> void {
    for(int x=0;x<SCREEN_WIDTH*8;x++) {
        for(int y=0;y<SCREEN_HEIGHT*8;y++) {
            plot(x,y,0);
        }
    }
    for(int x=0;x<SCREEN_WIDTH;x++)
        for(int y=0;y<SCREEN_HEIGHT;y++)
            buffer[x][y]=0;
    x=0;y=0;
}
auto Display::scroll() -> void {
    for(int x=0;x<SCREEN_WIDTH;x++) {
        for(int y=0;y<SCREEN_HEIGHT-1;y++) {
            buffer[x][y]=buffer[x][y+1];
        }
        buffer[x][SCREEN_HEIGHT-1]=0;
    }
    for(int bx=0;bx<SCREEN_WIDTH;bx++) {
        for(int by=0;by<SCREEN_HEIGHT;by++) {
            for(int x=0;x<8;x++) {
                for(int y=0;y<8;y++) {
                    plot(bx*8+x,by*8+y,(font[buffer[bx][by]][y]&(1<<x))?0xFFFFFF:0);
                }
            }
        }
    }
    x=0;
    y--;
}
template <>
auto Display::operator<<<uint64_t>(uint64_t i) -> Display & {
    //base is 16
    uint64_t output=i;
    const char* chars="0123456789ABCDEF";
    char buf[65];
    buf[64]='\0';
    char* ptr=buf+63;
    do {
        *(ptr--)=chars[output&0xF];
        output>>=4;
    } while(output && (ptr!=buf));
    puts(ptr+1);
    return *this;
}
auto Display::puts(const char * str) -> void {
    for(int i=0;str[i];i++) {
        putChar(str[i]);
    }
}
}
