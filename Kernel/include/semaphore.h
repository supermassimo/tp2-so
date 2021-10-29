#include <scheduler.h>
#include <memManager.h>

typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES] = {0};
    int waitingProcessesCount = 0;
} sem_t


