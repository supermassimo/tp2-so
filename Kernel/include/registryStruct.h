#include <stdint.h>

struct registry_struct {
    void* rax;
    void* rbx;
    void* rcx;
    void* rdx;
    void* rbp;
    void* rdi;
    void* rsi;
    void* r8;
    void* r9;
    void* r10;
    void* r11;
    void* r12;
    void* r13;
    void* r14;
    void* r15;
};
typedef struct registry_struct Registries;