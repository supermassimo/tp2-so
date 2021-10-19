#include <stddef.h>
#include <stdint.h>
#include <memManager.h>

void createProcess();
uint64_t* schedule(uint64_t* currentProcPCB);