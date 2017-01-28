CC=$(DEVKITARM)/bin/arm-none-eabi-gcc
LD=$(DEVKITARM)/bin/arm-none-eabi-gcc
ASFLAGS=-Wno-error -mcpu=arm946e-s -march=armv5te
CFLAGS=-Wno-error -I$(DEVKITPRO)/portlibs/armv5te/include -I$(DEVKITPRO)/portlibs/armv5te/include/freetype2 -std=gnu11 -mcpu=arm946e-s -march=armv5te -flto -mthumb

LDFLAGS=-L$(DEVKITPRO)/portlibs/armv5te/lib -L$(DEVKITARM)/arm-none-eabi/lib/ -lz -lc -lm -Tbootloader.ld -flto
SRCS=$(shell find -name '*.[cS]')
OBJS=$(addsuffix .o,$(basename $(SRCS)))
arm9loaderhax_si.bin: arm9loaderhax.elf
	$(DEVKITARM)/bin/arm-none-eabi-objcopy -O binary $^ $@
arm9loaderhax.elf: $(OBJS) $(DEVKITPRO)/portlibs/armv5te/lib/libbz2.a $(DEVKITPRO)/portlibs/armv5te/lib/libctr9.a $(DEVKITPRO)/portlibs/armv5te/lib/libctrelf.a $(DEVKITPRO)/portlibs/armv5te/lib/libctr_core.a $(DEVKITPRO)/portlibs/armv5te/lib/libfreetype.a $(DEVKITPRO)/portlibs/armv5te/lib/libpng.a $(DEVKITPRO)/portlibs/armv5te/lib/libz.a $(DEVKITARM)/arm-none-eabi/lib/libc.a $(DEVKITARM)/arm-none-eabi/lib/libm.a
	$(LD) $(LDFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
%.o: %.S
	$(CC) $(ASFLAGS) -c -o
