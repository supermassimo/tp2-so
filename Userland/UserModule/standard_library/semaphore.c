// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/semaphore.h"

extern int semInit (int value);
extern int semDestroy(int sem_id);
extern void semWait(int sem_id);
extern void semPost(int sem_id);

int binary_semaphore_init(){
    return semaphore_init(1);
}

// May return null if malloc fails
int semaphore_init(int value){
    return semInit(value);
}

//free
int semaphore_destroy(int sem_id){
    semDestroy(sem_id);
}

//wait then decrement
int semaphore_wait(int sem_id){
    semWait(sem_id);
}

//increment
int semaphore_post(int sem_id){
    semPost(sem_id);
}