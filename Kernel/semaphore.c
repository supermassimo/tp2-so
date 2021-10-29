#include <semaphore.h>

#define MAX_SEMAPHORES  1000

static sem_t semaphores[MAX_SEMAPHORES] = {0};

/*
static void sleepCurrent(sem_t sem){
    int pid = getpid();
    sem.waitingProcesses[sem.waitingProcessesCount++] = pid;
    sleep(pid); //sleep indefinido, necesita wakeup()
}

sem_t sem_init(int value){
    sem_t *sem = memAlloc(sizeof(sem_t), 0);
    if (sem == NULL)
        return NULL;
    (*sem).value = value;
    return *sem;
}

//free
int sem_destroy(sem_t sem){
    return memFree(*sem);
}

//sleep if semaphore is busy
void sem_wait(sem_t sem){
    if (sem.value <= 0){
        sleepCurrent(sem);
    }
    sem.value--;
}

//increment
void sem_post(sem_t sem){
    sem.value++;
    wakeup(sem);
}

void sem_set_value (sem_t sem, int value){
    sem.value = value;
}
*/