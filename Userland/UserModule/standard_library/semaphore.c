// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/semaphore.h"

extern sem_t* semInit (int value);
extern int semDestroy(sem_t sem);
extern void semWait(sem_t sem);
extern void semPost(sem_t sem);

sem_t binary_semaphore_init(){
    return semaphore_init(1);
}

// May return null if malloc fails
sem_t semaphore_init(int value){
    return *semInit(value);
}

//free
int semaphore_destroy(sem_t sem){
    semDestroy(sem);
}

//wait then decrement
int semaphore_wait(sem_t sem){
    semWait(sem);
}

//increment
int semaphore_post(sem_t sem){
    semPost(sem);
}