#include <stdint.h>

struct registry_struct {
    int64_t rax;
    int64_t rbx;
    int64_t rcx;
    int64_t rdx;
    int64_t rbp;
    int64_t rdi;
    int64_t rsi;
    int64_t r8;
    int64_t r9;
    int64_t r10;
    int64_t r11;
    int64_t r12;
    int64_t r13;
    int64_t r14;
    int64_t r15;
};
typedef struct registry_struct Registries;