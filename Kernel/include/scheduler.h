#include <stddef.h>
#include <stdint.h>
#include <memManager.h>
#include <lib.h>

typedef enum Priority {LOW, MEDIUM, HIGH, SYSTEM} Priority;
typedef enum ProcessSignal {SIG_KILL} ProcessSignal;

void enableScheduler();
int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name);
int nice(int pid, Priority priority);
int block(int pid);

void exit(int status);
int kill(int pid, ProcessSignal sig);
uint64_t* schedule(uint64_t* currentProcPCB);