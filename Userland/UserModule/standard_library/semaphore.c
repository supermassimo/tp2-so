// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/semaphore.h"

extern int semInit(const char *sem_id, int value);
extern int semDestroy(const char *sem_id);
extern int semWait(const char *sem_id);
extern int semPost(const char *sem_id);

// May return null if malloc fails
int semaphore_init(const char *sem_id, int value){
    return semInit(sem_id, value);
}

//free
int semaphore_destroy(const char *sem_id){
    semDestroy(sem_id);
}

//wait then decrement
int semaphore_wait(const char* sem_id){
    semWait(sem_id);
}

//increment
int semaphore_post(const char* sem_id){
    semPost(sem_id);
}