#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#define MAX_PROCESSES 10

typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES];
    int waitingCount;
} sem_t;

sem_t binary_semaphore_init();
sem_t semaphore_init(int value);
int semaphore_destroy(sem_t sem);
int semaphore_wait(sem_t sem);
int semaphore_post(sem_t sem);

#endif