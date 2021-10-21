#include <stddef.h>
#include <stdint.h>
#include <memManager.h>

void enableScheduler();
int createProcess(void* entryPoint, int argc, char** argv);
uint64_t* schedule(uint64_t* currentProcPCB);