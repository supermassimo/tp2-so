#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include <stdint.h>
#include <memManager.h>
#include <lib.h>
#include <time.h>
#include <console.h>

#define PROCESS_STACK  4080         // PAGE_SIZE - MEM_HEADER_SIZE

typedef enum Priority {LOW, MEDIUM, HIGH, SYSTEM} Priority;
typedef enum ProcessSignal {SIG_KILL} ProcessSignal;
typedef enum State {TERMINATED, READY, BLOCKED, SLEEP} State;

typedef struct Process {
    char* name;
    State state;
    Priority priority;
    uint64_t *pcb;
    uint64_t *base;
    char** argv;
    size_t sleepTime;
} Process;

void enableScheduler();
int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name);
int nice(int pid, Priority priority);
int block(int pid);
void printAllProcesses();
char* getStateString(State state);
int scheduleOutsideRtc();
int sleep(int pid, size_t seconds);

void exit(int status);
int kill(int pid, ProcessSignal sig);
int getpid();
uint64_t* schedule(uint64_t* currentProcPCB);

#endif