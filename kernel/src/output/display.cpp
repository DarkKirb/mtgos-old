#include "display.hpp"
#include <defines.h>
uint16_t *font[65536]={nullptr};
struct fontEntry {
    int width;
    int codepoint;
    uint16_t bitmap[16];
};
struct fontHeader {
    char magic[4];
    int charno;
    int height;
    int zero;
    fontEntry chars[];
};
namespace MTGos {

Display::Display(multiboot_info_t* mb_info): mb_info(mb_info), x(0),y(0) {
    for(int x=0;x<SCREEN_WIDTH;x++)
        for(int y=0;y<SCREEN_HEIGHT;y++)
            buffer[x][y]=0;
    //Font has to be the first module
    multiboot_module_t *mods = (multiboot_module_t*)((uintptr_t)(mb_info->mods_addr));
    fontHeader* f = (fontHeader*)((uintptr_t)(mods[0].mod_start));
    if((f->magic[0] != 'F') || (f->magic[1] != 'O') || (f->magic[2] != 'N') || (f->magic[3] != 'T')) {
        //Not a font
        //for(;;);
    }
    for(int i=0;i<65536;i++)
        font[i]=nullptr;
    for(int i=0;i<f->charno;i++) {
        if(f->chars[i].codepoint > 65535)
            continue;
        font[f->chars[i].codepoint]=&(f->chars[i].bitmap[0]);
    }
}
Display::~Display() {}
auto Display::putChar(int c) -> void {
    if(c<0) {
        return;
    }
    if(c>0xFFFF)
        return;
    if(!font[c])
        return;
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
        //Decode UTF-8
        int character=0;
        if(str[i]<0x80) {
            character=str[i];
        }
        //UTF-8 character
        if((str[i]&0b11000000)==0b10000000) {
            character=0xFFFD; //ILLEGAL SEQUENCE
        }
        //Now count how many bytes follow
        int bytesToFollow=0;
        if((str[i]&0b11100000)==0b11000000) {
            bytesToFollow=1;
            character=str[i]&0b11111;
        } else if((str[i]&0b11110000)==0b11100000) {
            bytesToFollow=2;
            character=str[i]&0b1111;
        } else if((str[i]&0b11111000)==0b11110000) {
            bytesToFollow=3;
            character=str[i]&0b111;
        } else if(!character)
            character=0xFFFD; //ILLEGAL SEQUENCE
        while(bytesToFollow) {
            i++;
            if(!str[i]) {
                putChar(0xFFFD); //Incomplete
                return;
            }
            if((str[i]&0b11000000)!=0b10000000) {
                putChar(0xFFFD); //ILLEGAL SEQUENCE
                break;
            }
            character <<= 6;
            character |= str[i] & 0b111111;
            bytesToFollow--;
        }
        if(!bytesToFollow)
            putChar(character);
    }
}
auto Display::plotChar(int x, int y, int c) -> void {
    if(buffer[x][y]==c)
        return;
    for(int cx=0;cx<16;cx++) {
        for(int cy=0;cy<16;cy++) {
            plot(x*16+cx,y*16+cy,(font[c][cy]&(1<<cx))?0xFFFFFF:0);
        }
    }
    buffer[x][y]=c;
}
}
