#include <stdint.h>

typedef struct Registries {
    uint64_t RAX;
    uint64_t RBX;
    uint64_t RCX;
    uint64_t RDX;
    uint64_t RDI;
    uint64_t RSI;
    uint64_t R8;
    uint64_t R9;
    uint64_t R10;
    uint64_t R11;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;
    uint64_t RBP;
    uint64_t RSP;
    uint64_t RIP
} Registries;