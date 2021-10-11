#include <scheduler.h>

#define MAX_PROCESSES 10

typedef enum State {READY, BLOCKED, TERMINATED} State;

typedef struct {
    uint64_t ss;
    uint64_t rsp;
    uint64_t rflags;
    uint64_t cs;
    uint64_t rip;
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rsp;
} PCB;

typedef struct {
    PCB pcb;
    State state;
} Process;

static Process *processes;
static int activeProcessIdx;

void createFirstProcess(void* entryPoint){
    processes = (Process*) memAlloc(sizeof(Process)*MAX_PROCESSES, SET_ZERO);
    processes[0].pcb.rflags = 0x202;
    processes[0].pcb.rip = &entryPoint;
    processes[0].state = READY;
    activeProcessIdx = 0;
}

uint64_t schedule (uint64_t pcb){
    for(int i=activeProcessIdx+1 ; i % MAX_PROCESSES != activeProcessIdx ; i++){
        if(processes[i%MAX_PROCESSES].state == READY)
    }
}