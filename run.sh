#!/bin/bash
if [[ "$1" = "gdb" ]]; then
    qemu-system-x86_64 -curses -s -S -cpu kvm64,+mmx,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+aes,+pclmulqdq,+avx,+vaes,+vpclmulqdq,+f16c,+fma,+avx2 -hda Image/x64BareBonesImage.qcow2 -m 512 -d int
else
    qemu-system-x86_64 -curses -cpu kvm64,+mmx,+sse,+sse2,+sse3,+sse4.1,+sse4.2,+aes,+pclmulqdq,+avx,+vaes,+vpclmulqdq,+f16c,+fma,+avx2 -hda Image/x64BareBonesImage.qcow2 -m 2048M
fi
