// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/semaphore.h"

extern int semOpen(const char *sem_id, int value);
extern int semDestroy(const char *sem_id);
extern int semWait(const char *sem_id);
extern int semPost(const char *sem_id);

// May return null if malloc fails
int sem_open(const char *sem_id, int value){
    return semOpen(sem_id, value);
}

//free
int sem_destroy(const char *sem_id){
    return semDestroy(sem_id);
}

//wait then decrement
int sem_wait(const char* sem_id){
    return semWait(sem_id);
}

//increment
int sem_post(const char* sem_id){
    return semPost(sem_id);
}