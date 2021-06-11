#!/bin/bash
qemu-system-x86_64 -cpu qemu64,mmx=on,sse=on,sse2=on,sse3=on,sse4.1=on,sse4.2=on,aes=on,pclmulqdq=on,avx=on,vaes=on,vpclmulqdq=on,f16c=on,fma=on,avx2=on -hda Image/x64BareBonesImage.qcow2 -m 512 
