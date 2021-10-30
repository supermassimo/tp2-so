// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore.h>

#define MAX_SEMAPHORES  1000

static sem_t semaphores[MAX_SEMAPHORES] = {0};

static void sleepCurrent(sem_t sem){
    int pid = getpid();
    if (sem.nextSleep == MAX_PROCESSES){
        return; // no hay mas espacio para procesos
    } 
    sem.waitingProcesses[sem.waitingCount++] = pid;
    makeWait(pid); //sleep indefinido, necesita wakeup()
}

// despierta a un proceso en la lista, funciona como stack pero sería mejor como queue (?)
static void wakeupNext(sem_t sem){
    if (sem.waitingCount > 0){
        int nextPid = sem.waitingProcesses[sem.waitingCount--];
        wakeup(nextPid);
    }
}

sem_t sem_init(int value){
    sem_t *sem = memAlloc(sizeof(sem_t), 0);
    if (sem == NULL)
        return NULL;
    sem->value = value;
    sem->waitingProcesses = {0};
    sem->waitingProcessCount = 0;
    sem->pointerNext = 0;
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
    wakeupNext(sem);
}

void sem_set_value (sem_t sem, int value){
    sem.value = value;
}
