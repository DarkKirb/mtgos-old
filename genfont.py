#!/usr/bin/env python3
import binascii
f=open("font.bin","wb")
f2=open("font.big","wb")
f.write(b"TNOF")
f2.write(b"FONT")
fin=open("unifont.hex","r")
lines=fin.readlines()
font={}
for l in lines:
    font[int.from_bytes(binascii.unhexlify(
        l[:4]), byteorder="big")] = binascii.unhexlify(l[5:-1])
f.write(len(font).to_bytes(4, byteorder="little"))
f.write(int(16).to_bytes(4, byteorder="little"))
f.write(int(0).to_bytes(4, byteorder="little"))
f2.write(len(font).to_bytes(4, byteorder="big"))
f2.write(int(16).to_bytes(4, byteorder="big"))
f2.write(int(0).to_bytes(4, byteorder="big"))
for id, fo in font.items():
    fullwidth = True
    if len(fo) == 16:
        fullwidth = False
        temp = b''
        for b in fo:
            temp += bytes([b, 0])
        fo = temp
        f.write(int(8).to_bytes(4, byteorder="little"))
        f2.write(int(8).to_bytes(4, byteorder="big"))
    else:
        f.write(int(16).to_bytes(4, byteorder="little"))
        f2.write(int(16).to_bytes(4, byteorder="big"))
    temp=b''
    for i in range(16):
        b='{:016b}'.format(int.from_bytes(bytes([fo[i*2+1],fo[i*2]]),byteorder="little"))
        temp+=int(b[::-1],2).to_bytes(2,byteorder="little")
    f.write(id.to_bytes(4,byteorder="little"))
    f2.write(id.to_bytes(4,byteorder="big"))
        b='{:016b}'.format(int.from_bytes(bytes([fo[i*2+1],fo[i*2]]),byteorder="little"))
        temp+=int(b[::-1],2).to_bytes(2,byteorder="little")
    f.write(id.to_bytes(4,byteorder="little"))
        b = '{:016b}'.format(int.from_bytes(
            bytes([fo[i * 2 + 1], fo[i * 2]]), byteorder="little"))
        temp += int(b[::-1], 2).to_bytes(2, byteorder="little")
    f.write(id.to_bytes(4, byteorder="little"))
    f.write(temp)
    f2.write(temp)
f.close()
f2.close()
fin.close()
