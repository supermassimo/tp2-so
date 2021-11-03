#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stddef.h>
#include <stdint.h>
#include <memManager.h>
#include <lib.h>
#include <time.h>
#include <console.h>
#include <keyboard.h>

typedef enum Priority {LOW, MEDIUM, HIGH, SYSTEM} Priority;
typedef enum ProcessSignal {SIG_KILL} ProcessSignal;
typedef enum State {TERMINATED, READY, BLOCKED, SLEEP, WAITING, TO_TERMINATE} State;
typedef enum BlockOption {BLOCK, UNBLOCK} BlockOption;

typedef struct {
    char* name;
    State state;
    Priority priority;
    uint64_t *pcb;
    uint64_t *base;
    char** argv;
    size_t sleepTime;
} Process;

#define MAX_PROCESSES   100
#define PROCESS_STACK  8176        // (2 * PAGE_SIZE) - MEM_HEADER_SIZE

void enableScheduler();
void createHaltingProcess();

int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name);
int nice(int pid, Priority priority);
int block(int pid, BlockOption option);
void printAllProcesses();
char* getStateString(State state);
int scheduleOutsideRtc();
int sleep(int pid, size_t seconds);
int makeWait(int pid);
int wakeup(int pid);
void skip();

void exit(int status);
int kill(int pid, ProcessSignal sig);
int getpid();
uint64_t* schedule(uint64_t* currentProcPCB);

void printProcesses(Process* processes, size_t amount);

#endif