CROSSPATH = $(HOME)/opt
TARGET = none
export PATH := $(CROSSPATH)/opt/bin:$(PATH)
all: | clean $(CROSSPATH)/ fetch patch prereqs i686-elf-gcc arm-none-eabi-gcc armeb-eabi-gcc ppc-elf-gcc aarch64-none-elf-gcc x86-64-elf-gcc
	
clean:
	rm -rf $(CROSSPATH)
	rm -rf temp

$(CROSSPATH)/:
	mkdir -p $(CROSSPATH)
	mkdir -p $(CROSSPATH)/include
	cp -rv ../libk/include/* $(CROSSPATH)/include

fetch:
	cd temp; \
	wget https://ftp.gnu.org/gnu/gawk/gawk-4.1.4.tar.xz https://ftp.gnu.org/gnu/gcc/gcc-6.2.0/gcc-6.2.0.tar.bz2 https://ftp.gnu.org/gnu/binutils/binutils-2.27.tar.bz2 https://ftp.gnu.org/gnu/mpc/mpc-1.0.3.tar.gz https://ftp.gnu.org/gnu/mpfr/mpfr-3.1.5.tar.xz https://ftp.gnu.org/gnu/gmp/gmp-6.1.1.tar.xz http://isl.gforge.inria.fr/isl-0.17.tar.xz http://www.bastoul.net/cloog/pages/download/cloog-0.18.4.tar.gz; \
	echo "GAWK"; \
	tar -xf gawk-4.1.4.tar.xz; \
	echo "GCC"; \
	tar -xf gcc-6.2.0.tar.bz2; \
	echo "binutils"; \
	tar -xf binutils-2.27.tar.bz2; \
	cd gcc-6.2.0; \
	echo "MPC"; \
	tar -xf ../mpc-1.0.3.tar.gz; \
	echo "MPFR"; \
	tar -xf ../mpfr-3.1.5.tar.xz; \
	echo "GMP"; \
	tar -xf ../gmp-6.1.1.tar.xz; \
	echo "ISL"; \
	tar -xf ../isl-0.17.tar.xz; \
	echo "CLOOG"; \
	tar -xf ../cloog-0.18.4.tar.gz; \
	mv mpc-* mpc; \
	mv mpfr-* mpfr; \
	mv gmp-* gmp; \
	mv isl-* isl; \
	mv cloog-* cloog;
patch:
	sed -i -e '/No support for this/d' temp/gcc-6.2.0/libstdc++-v3/configure
prereqs:
	cd temp/gawk-4.1.4; \
	./configure --prefix=$(CROSSPATH); \
	$(MAKE); \
	$(MAKE) install

i686-elf-gcc-set:
	$(eval TARGET = i686-elf)
i686-elf-gcc: | i686-elf-gcc-set build
arm-none-eabi-set:
	$(eval TARGET = arm-none-eabi)
arm-none-eabi-gcc: | arm-none-eabi-set build
armeb-eabi-gcc-set:
	$(eval TARGET = armeb-eabi)
armeb-eabi-gcc: | armeb-eabi-gcc-set build
ppc-elf-gcc-set:
	$(eval TARGET = ppc-elf)
ppc-elf-gcc: | ppc-elf-gcc-set build
aarch64-none-elf-gcc-set:
	$(eval TARGET = aarch64-none-elf)
aarch64-none-elf-gcc: | aarch64-none-elf-gcc-set build
x86-64-elf-gcc-set:
	$(eval TARGET = x86_64-elf)
x86-64-elf-gcc: | x86-64-elf-gcc-set build
x86-64-w64-mingw32-gcc-set:
	$(eval TARGET = x86_64-w64-mingw32)
x86-64-w64-mingw32-gcc: | x86-64-w64-mingw32-gcc-set build
build:
	cd temp && \
	mkdir build-binutils && \
	cd build-binutils && \
	../binutils-2.27/configure --prefix=$(CROSSPATH) --target=$(TARGET) --with-sysroot --disable-nls --disable-werror && \
	$(MAKE) && \
	$(MAKE) install && \
	cd .. && \
	rm -rf build-binutils && \
	mkdir build-gcc && \
	cd build-gcc && \
	../gcc-6.2.0/configure --prefix=$(CROSSPATH) --target=$(TARGET) --disable-nls --enable-languages=c,c++ --without-headers && \
	$(MAKE) all-gcc all-target-libgcc && \
	$(MAKE) install-gcc install-target-libgcc && \
	cd ../../../libk && \
	TARGET=$(TARGET) $(MAKE) -j8 && \
	cp libc.a $(CROSSPATH)/lib/gcc/$(TARGET)/6.2.0/libc.a && \
	$(MAKE) clean && \
	cp -rv include/* $(CROSSPATH)/lib/gcc/$(TARGET)/6.2.0/include && \
	cd ../cross/temp/build-gcc && \
	mkdir lsc++ && \
	cp -v ../../config.cache lsc++/config.cache && \
	cd lsc++ && \
	cp -v ../../../config.cache . && \
	mkdir -pv ../thumb/lsc++ ../fpu/lsc++ && \
	cp config.cache ../thumb/lsc++ && \
	cp config.cache ../fpu/lsc++ && \
	CPP=$(TARGET)-cpp ../../gcc-6.2.0/libstdc++-v3/configure --host=$(TARGET) --prefix=$(CROSSPATH) --disable-hosted-libstdcxx --disable-nls -C && \
	cd include && \
	$(MAKE) && \
	$(MAKE) install && \
	cd ../libsupc++ && \
	$(MAKE) && \
	$(MAKE) install && \
	cd ../.. && \
	cd .. && \
	rm -rf build-gcc
	mv $(CROSSPATH)/lib/libsupc++.* $(CROSSPATH)/lib/gcc/$(TARGET)/6.2.0/
	@echo "Cross-compiler for $(TARGET) was built."

.PHONY: clean all fetch build
.NOTPARALLEL:
