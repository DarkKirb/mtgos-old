#!/usr/bin/env python3
import binascii
f=open("font.bin","wb")
f.write(b"FONT")
fin=open("unifont.hex","r")
lines=fin.readlines()
font={}
for l in lines:
    font[int.from_bytes(binascii.unhexlify(l[:4]),byteorder="big")] = binascii.unhexlify(l[5:-1])
f.write(len(font).to_bytes(4, byteorder="little"))
f.write(int(16).to_bytes(4, byteorder="little"))
f.write(int(0).to_bytes(4, byteorder="little"))
for id, fo in font.items():
    if len(fo)==16:
        temp=b''
        for b in fo:
            temp += bytes([b]) + bytes(1)
        fo=temp
    temp=b''
    for i in range(16):
        b='{:016b}'.format(int.from_bytes(bytes([fo[i*2+1],fo[i*2]]),byteorder="little"))
        temp+=int(b[::-1],2).to_bytes(2,byteorder="little")

    f.write(int(16).to_bytes(4, byteorder="little"))
    f.write(id.to_bytes(4,byteorder="little"))
    f.write(temp)
f.close()
fin.close()