#include <pipe.h>


static Pipe *first = NULL;
static Pipe *last = NULL;
static int pipeAmount = 0;
static int id = 0;

int createPipe() {
    Pipe *pipe = memAlloc(sizeof(Pipe),0);
    pipe->id = id;
    pipe->nwrite = 0;
    pipe->nread = 0;
    pipe->sem = sem_init(1);
    pipe->next = NULL;
    if(first == NULL) {
        first = pipe;
    } else {
        last->next = pipe;
    }
    last = pipe;
    pipeAmount++;
    return id++;
}

void freePipe(Pipe *p) {
    sem_destroy(p->sem);
    p->next = NULL;
    memFree(p);
    return;
}

void closePipe(int id) {
    if(first == NULL) {
        return;
    }
    Pipe *aux;
    if(first->id == id) {
        aux = first;
        first = aux->next;
        //freePipe(aux);
        pipeAmount--;
        return;
    }
    Pipe *p = first;
    for(int i=1; i<pipeAmount; i++) {
        if(p == NULL) {
            return;
        } 
        if(p->next != NULL && p->next->id == id) {
            aux = p->next;
            p->next = aux->next;
            if(p->next == NULL) {
                last = p;
            }
            //freePipe(aux);
            pipeAmount--;
            return;
        }
    }
    return;
}

int writePipe(int id, char* const buf, int count) {
    Pipe *p = first;
    for(int i=0; i < pipeAmount; i++) {
        if(p == NULL) {
            return -1;
        } 
        if(p->id == id) {
            break;
        } 
        p = p->next;
    }
    sem_wait(p->sem);
    print("Writing pipe\n\n");
    print("     Before: ");
    print(buf);
    for(int i=0; p->nwrite < BUFFER_SIZE && i <= count; i++) {
        p->buffer[p->nwrite++] = buf[i];
    }
    print("     After: ");
    print(p->buffer);
    print("\n");
    sem_post(p->sem);
    if(p->nwrite == BUFFER_SIZE) {
        return -1;
    }
    return 0;
}

int readPipe(int id, char* buf, int count) {
    Pipe *p = first;
    for(int i=0; i < pipeAmount; i++) {
        if(p == NULL) {
            return -1;
        } 
        if(p->id == id) {
            break;
        } 
        p = p->next;
    }
    sem_wait(p->sem);
    print("Reading pipe\n\n");
    print("     Before: ");
    print(p->buffer);
    for(int i=0; p->nread < BUFFER_SIZE && i <= count; i++) {
        buf[i] = p->buffer[p->nread++];
    }
    print("     After: ");
    print(p->buffer);
    print("\n");
    sem_post(p->sem);
    if(p->nread == BUFFER_SIZE) {
        return -1;
    }
    return 0;
}

void printPipe(Pipe *pipe) {
    changeConsoleSide(1);
    print("Pipe = ");
    printInt(pipe,16);
    print("\nID = ");
    printInt(pipe->id,10);
    print("\nsem.value = ");
    printInt(pipe->sem.value,10);
    print("\nnwrite = ");
    printInt(pipe->nwrite,10);
    print("\nnread = ");  
    printInt(pipe->nread,10);
    print("\nnext = ");
    printInt(pipe->next,16);
    print("\n\n");
    changeConsoleSide(0);
    return;
}

void printPipes() {
    Pipe *p = first;
    //print("Pipe PID IO State\n");
    for(int i=0; i<pipeAmount; i++) {
        if(p == NULL) {
            return;
        } else {
            /*printInt(p->id,10);
            print("    \n");*/
            printPipe(p);
            p = p->next;
        }
    }
    return;
}