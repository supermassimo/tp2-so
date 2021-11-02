#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_


int semaphore_init(const char *sem_id, int value);
int semaphore_destroy(const char *sem_id);
int semaphore_wait(const char* sem_id);
int semaphore_post(const char* sem_id);


#endif