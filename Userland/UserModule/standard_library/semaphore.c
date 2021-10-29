#include "./include/semaphore.h"

sem_t *binary_semaphore_init(){
    return semaphore_init(1);
}

// May return null if malloc fails
sem_t *semaphore_init(int value){
    //syscall init
}

//free
int semaphore_destroy(sem_t *sem){
    //syscall destroy
}

//wait then decrement
int semaphore_wait(sem_t *sem){
    //syscall wait
}

//increment
int semaphore_post(sem_t *sem){
    //syscall post
}