#!/usr/bin/env bash
mkdir mtgos-builds
sudo cp font.bin mtgos-builds
echo "Building for i686"
cp templates/i686.yaml config.yaml
python3.4 ./make.py --clean --ci
sudo mv bootable.iso mtgos-builds/i686.iso
sudo mv mtgos.elf mtgos-builds/i686.elf
echo "Building for x86_64"
cp templates/x86_64.yaml config.yaml
python3.4  ./make.py --clean --ci
sudo mv bootable.iso mtgos-builds/x86_64.iso
sudo mv mtgos.elf mtgos-builds/x86_64.elf
echo "Building for 3DS"
sudo cp bootloader/3ds/arm9loaderhax_si.bin mtgos-builds/arm9loaderhax_si.bin
cp templates/3ds9.yaml config.yaml
python3.4  ./make.py --clean --ci
sudo mv mtgos.elf mtgos-builds/kernel9.elf
cp templates/3ds11.yaml config.yaml
python3.4 ./make.py --clean --ci
sudo mv mtgos.elf mtgos-builds/kernel11.elf
