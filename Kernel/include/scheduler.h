#include <stddef.h>
#include <stdint.h>
#include <memManager.h>
#include <lib.h>

typedef enum Priority {LOW, MEDIUM, HIGH, SYSTEM} Priority;

void enableScheduler();
int createProcess(void* entryPoint, Priority priority, int argc, char* argv[], char* name);
uint64_t* schedule(uint64_t* currentProcPCB);