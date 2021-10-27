#include "./include/semaphore.h"
#include "./include/mystdlib.h"

#define getValue(X) (*((int*)(X)))

sem_t *binary_semaphore_init(){
    return semaphore_init(1);
}

// May return null if malloc fails
sem_t *semaphore_init(int value){
    sem_t sem = malloc(sizeof(int));
    if (sem == NULL)
        return NULL;
    getValue(sem) = value;
    return sem;
}

//free
int semaphore_destroy(sem_t *sem){
    return free(sem);
}

//wait then decrement
int semaphore_wait(sem_t *sem){
    while(getValue(sem) <= 0);
    getValue(sem) = getValue(sem) - 1;
    return 0;
}

//increment
int semaphore_post(sem_t *sem){
    getValue(sem) = getValue(sem) + 1;
    return 0;
}