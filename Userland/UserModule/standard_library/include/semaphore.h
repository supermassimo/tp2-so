#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

typedef void* sem_t;

sem_t *semaphore_init(int value);
int semaphore_destroy(sem_t *sem);
int semaphore_wait(sem_t *sem);
int semaphore_post(sem_t *sem);

#endif