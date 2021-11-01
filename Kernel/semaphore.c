// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore.h>

#define MAX_SEMAPHORES  1000

typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES];
    int waitingCount;
} sem_t;

static sem_t* semaphores[MAX_SEMAPHORES] = {0};
static int max_semaphore_id = 0;

static void sleepCurrent(sem_t sem){
    int pid = getpid();
    if (sem.waitingCount == MAX_PROCESSES){
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

int sem_init(int value){
    sem_t *sem = memAlloc(sizeof(sem_t), 0);
    
    if (sem == NULL)
         return -1; //tirar algun tipo de error (?)
    
    sem->value = value;
    sem->waitingCount = 0;
    
    int i;
    for (i=0;i<MAX_PROCESSES;i++){
        sem->waitingProcesses[i] = 0;
        if (semaphores[i] == 0 || semaphores[i] == NULL){
            semaphores[i] = sem;
            if (i > max_semaphore_id){
                max_semaphore_id = i;
            }
            break;
        }
    }

    if (max_semaphore_id >= MAX_SEMAPHORES){
        return -1;
    }
    
    return i;
}

//free
int sem_destroy(int sem_id){
    sem_t* sem = semaphores[sem_id];
    int ret = memFree(sem);
    semaphores[sem_id] = NULL;
    return ret;
}

//sleep if semaphore is busy
void sem_wait(int sem_id){
    sem_t sem = *semaphores[sem_id];
    if (sem.value <= 0){
        sleepCurrent(sem);
    }
    sem.value--;
}

//increment
void sem_post(int sem_id){
    sem_t sem = *semaphores[sem_id];
    sem.value++;
    wakeupNext(sem);
}

void sem_set_value (int sem_id, int value){
    sem_t sem = *semaphores[sem_id];
    sem.value = value;
}
