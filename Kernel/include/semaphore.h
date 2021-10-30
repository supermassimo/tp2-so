#include <scheduler.h>
#include <memManager.h>

typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES];
    int waitingCount;
} sem_t;

sem_t sem_init(int value);
int sem_destroy(sem_t sem);
void sem_wait(sem_t sem);
void sem_post(sem_t sem);
void sem_set_value (sem_t sem, int value);
