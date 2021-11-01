#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#define MAX_PROCESSES 10

typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES];
    int waitingCount;
} sem_t;

int binary_semaphore_init();
int semaphore_init(int value);
int semaphore_destroy(int sem_id);
int semaphore_wait(int sem_id);
int semaphore_post(int sem_id);

#endif