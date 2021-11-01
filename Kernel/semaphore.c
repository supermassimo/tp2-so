// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore.h>

static sem_t *first = NULL;
static sem_t *last = NULL;
static int semAmount = 0;

static void printSemaphore(sem_t* s){
    print(s->id);
    print("\t\t");
    printInt(s->value, 10);
    print("\n");
}

void printAllSemaphores(){
    sem_t *s = first;
    print("Name\t\tValue\n");
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            break;
        }
        printSemaphore(s);
        s = s->next;
    }
}

int sem_init(const char *sem_id, int value) {
    size_t idLen = strlen(sem_id);
    sem_t *sem = memAlloc(sizeof(sem_t), NO_ACTION);
    if(sem == NULL || idLen > MAX_ID_LENGTH) {
        return -1;
    }
    // sem->id = sem_id;    //Copia de string esta mal
    sem->id = (char*)memAlloc(idLen+1, NO_ACTION);
    memcpy(sem->id, sem_id, idLen+1);
    sem->value = memAlloc(sizeof(int), NO_ACTION);
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
    printAllSemaphores();
    /*
    print("Creo semaforo ");
    print(sem->id);
    print("\ncon valor ");
    printInt(sem->value, 10);
    */
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

int sem_destroy(const char *sem_id) {
    if(first == NULL) {
        return -1;
    }
    sem_t *aux = first;
    if(strcmp(first->id,sem_id) == 0) {
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
        if(s->next != NULL && strcmp(s->next->id,sem_id) == 0) {
            aux = s->next;
            s->next = aux->next;
            if(s->next == NULL) {
                // last = p;
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
    printAllSemaphores();
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
        if(strcmp(s->id, sem_id) == 0) {       //comparacion de string esta mal
            print("ENCONTRE EL SEMAFORO ");
            print(s->id);
            print(" con ");
            printInt(s->value, 10);
            print("\n");
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

int sem_post(const char *sem_id) {
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return -1;
        } 
        if(strcmp(s->id, sem_id) == 0) {       //comparacion de string esta mal
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

int sem_set_value (const char *sem_id, int value) {
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return -1;
        }
        if(strcmp(s->id, sem_id) == 0) {
            s->value = value;
            break;
        }
        s = s->next;
    }
    return 0;
}

int sem_get_value (const char *sem_id){
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return -1;
        }
        if(strcmp(s->id, sem_id) == 0) {
            return s->value;
        }
        s = s->next;
    }
    return -1;
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