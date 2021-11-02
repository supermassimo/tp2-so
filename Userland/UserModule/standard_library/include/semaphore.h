#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_


int sem_open(const char *sem_id, int value);
int sem_destroy(const char *sem_id);
int sem_wait(const char* sem_id);
int sem_post(const char* sem_id);


#endif