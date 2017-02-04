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
    plotChar(x,y,c);
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
    for(int x=0;x<SCREEN_WIDTH;x++) {
        for(int y=0;y<SCREEN_HEIGHT;y++) {
            plotChar(x,y,0);
        }
    }
    x=0;y=0;
}
auto Display::scroll() -> void {
    for(int x=0;x<SCREEN_WIDTH;x++) {
        for(int y=0;y<SCREEN_HEIGHT-1;y++) {
            plotChar(x,y,buffer[x][y+1]);
        }
        plotChar(x,SCREEN_HEIGHT-1,0);
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
auto Display::plotChar(int x, int y, char c) -> void {
    if(buffer[x][y]==c)
        return;
    for(int cx=0;cx<8;cx++) {
        for(int cy=0;cy<8;cy++) {
            plot(x*8+cx,y*8+cy,(font[c][cy]&(1<<cx))?0xFFFFFF:0);
        }
    }
    buffer[x][y]=c;
}
}
