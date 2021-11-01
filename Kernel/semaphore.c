// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore.h>

static sem_t *first = NULL;
static sem_t *last = NULL;
static int semAmount = 0;

int sem_init(char *sem_id, int value) {
    sem_t *sem = memAlloc(sizeof(sem_t), 0);
    if(sem == NULL) {
        return -1;
    }
    sem->id = sem_id;    //Copia de string esta mal
    sem->value = value;
    sem->wp = NULL;
    sem->next = NULL;
    if(first == NULL) {
        first = sem;
    } else {
        last->next = sem;
    }
    last = sem;
    semAmount++;
    return 0;
}

void freeSem(sem_t *s) {
    WProcess *aux;
    while(s->wp != NULL) {
        aux = s->wp->next;
        s->wp->next = NULL;
        memFree(s->wp);
        s->wp = aux;
    }
    s->next = NULL;
    memFree(s);
}

int sem_destroy(char *sem_id) {
    if(first == NULL) {
        return -1;
    }
    sem_t *aux = first;
    if(first->id == sem_id) {
        aux = first;
        first = aux->next;
        //freeSem(aux);
        semAmount--;
        return;
    }
    sem_t *s = first;
    for(int i=1; i<semAmount; i++) {
        if(s == NULL) {
            return -1;
        } 
        if(s->next != NULL && s->next->id == sem_id) {
            aux = s->next;
            s->next = aux->next;
            if(s->next == NULL) {
                last = p;
            }
            //freeSem(aux);
            semAmount--;
            return 0;
        }
    }
    return 0;
}

static void sleepCurrent(sem_t *s){
    WProcess *wp = memAlloc(sizeof(WProcess),0);
    wp->pid = getpid();
    wp->next = NULL;
    if(s->wp == NULL) {
        s->wp = wp;
        makeWait(wp->pid);
        return;
    }
    WProcess *aux = s->wp;
    while(aux->next != NULL) {
        aux = aux->next;
    } 
    aux->next = wp;
    makeWait(wp->pid); //sleep indefinido, necesita wakeup()
    return;
}

int sem_wait(char *sem_id) {
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return -1;
        } 
        if(s->id == sem_id) {       //comparacion de string esta mal
            break;
        }
        s = s->next;
    }
    if(s->value <= 0) {
        sleepCurrent(s);
    }
    s->value--;
    return 0;
}

// despierta a un proceso en la lista
static void wakeupNext(sem_t *s){
    if(s->wp == NULL) {
        return;
    }
    int nextPid = s->wp->pid;
    s->wp = s->wp->next;
    wakeup(nextPid);
    return;
}

int sem_post(char *sem_id) {
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return -1;
        } 
        if(s->id == sem_id) {       //comparacion de string esta mal
            break;
        }
        s = s->next;
    }
    if(s->value <= 0) {
        s->value++;
        wakeupNext(s);
    } else {
        s->value++;
    }
    return 0;
}

int sem_set_value (char *sem_id, int value) {
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return -1;
        }
        if(s->id = sem_id) {
            s->value = value;
            break;
        }
        s = s->next;
    }
    return 0;
}



/*#define MAX_SEMAPHORES  1000

typedef struct {
    int value;
    int waitingProcesses[MAX_PROCESSES];
    int waitingCount;
} sem_t;

static sem_t *semaphores = NULL;
static int semAmount = 0;
static int max_semaphore_id = 0;

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
*/