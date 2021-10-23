#include <stddef.h>
#include <stdint.h>
#include <memManager.h>

typedef enum Priority {LOW, MEDIUM, HIGH, SYSTEM} Priority;

void enableScheduler();
int createProcess(void* entryPoint, Priority priority, int argc, char* argv[]);
uint64_t* schedule(uint64_t* currentProcPCB);