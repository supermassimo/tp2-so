// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <semaphore.h>

static sem_t *first = NULL;
static sem_t *last = NULL;
static int semAmount = 0;

int sem_init(const char *sem_id, int value) {
    size_t idLen = strlen(sem_id);
    sem_t *sem = memAlloc(sizeof(sem_t), NO_ACTION);
    if(sem == NULL || idLen > MAX_ID_LENGTH) {
        return -1;
    }
    // sem->id = sem_id;    //Copia de string esta mal
    sem->id = (char*)memAlloc(idLen+2, NO_ACTION);
    memcpy(sem->id, sem_id, idLen+2);
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
    while(aux->next != NULL) {
        aux = aux->next;
    } 
    aux->next = wp;
    makeWait(wp->pid); //sleep indefinido, necesita wakeup()
    return;
}

int sem_wait(const char *sem_id) {
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

void printWaitingProcess(const char *sem_id) {
    sem_t *s = first;
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            return;
        }
        if(strcmp(s->id, sem_id) == 0) {
            break;
        }
        s = s->next;
    }
    if(s->wp == NULL) {
        print("-");
        return;
    }
    WProcess *wp = s->wp;
    while(wp != NULL) {
        printInt(wp->pid,10);
        if(wp->next != NULL) {
            print(", ");
        }
    }
    return;
}

static void printSemaphore(sem_t* s){
    print(s->id);
    int len = strlen(s->id); 
    for(int i=0;i<15;i++) {
        if(i>len) {
            print(" ");
        }
    }
    printInt(s->value, 10);
    print("     ");
    printWaitingProcess(s->id);
    print("\n");
}

void printAllSemaphores(){
    if(first == NULL) {
        print("No sem open\n");
        return;
    }
    sem_t *s = first;
    print("Name          Value PID-Blocked\n");
    for(int i=0;i<semAmount;i++) {
        if(s == NULL) {
            break;
        }
        printSemaphore(s);
        s = s->next;
    }
}
