#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

typedef void* sem_t;

sem_t *semaphore_init(int value);
void semaphore_destroy(sem_t *sem);
void semaphore_wait(sem_t *sem);
void semaphore_post(sem_t *sem);

#endif