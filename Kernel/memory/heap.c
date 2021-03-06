// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memManager.h>

typedef struct MemHeader {
    struct MemHeader* next;
    size_t size;
} MemHeader;

static MemHeader baseHeader;
static MemHeader* freeList = NULL;

static MemHeader* getBlocks(size_t blockAmount){
    uint8_t *auxp;
    MemHeader *newHeader;
    size_t bytes = blockAmount * sizeof(MemHeader);
    
    if(bytes < PAGE_SIZE)           // Always reserve at least a page of memory
        bytes = PAGE_SIZE;
    if((auxp = memMap(bytes)) == NULL){
        return NULL;
    }
    newHeader = (MemHeader*)auxp;
    newHeader->size = bytes;
    memFree((void*)(newHeader+1));
    return freeList;                // Returns block added to free list
}

void* _memAlloc(size_t size){
    MemHeader *auxp, *currentBlock;
    size_t headerBlocks = (size + sizeof(MemHeader) - 1)/sizeof(MemHeader) + 1;         // Always round up # of block needed
    size_t blockBytes = headerBlocks * sizeof(MemHeader);

    if((currentBlock = freeList) == NULL){     // No free blocks to asign yet
        baseHeader.next = freeList = currentBlock = &baseHeader;
        baseHeader.size = 0;
    }

    for(auxp = currentBlock->next ;  ; currentBlock = auxp, auxp = auxp->next){
        if(auxp->size >= blockBytes){
            if(auxp->size == blockBytes)
                currentBlock->next = auxp->next;
            else {
                auxp->size -= blockBytes;
                auxp += auxp->size;
                auxp->size = blockBytes;
            }
            freeList = currentBlock;
            incrementOccupiedMemory(blockBytes);
            return (void*)(auxp+1);
        }
        if(auxp == freeList)
            if((auxp = getBlocks(headerBlocks)) == NULL)
                return NULL;                        // If system has no more free memory, return NULL
    }
}

/* free: put block ap in free list */
int _memFree(void *blockp) {
    MemHeader *headp, *p;
    headp = (MemHeader *)blockp - 1; /* point to block header */
    for (p = freeList; !(headp > p && headp < p->next); p = p->next)
        if (p >= p->next && (headp > p || headp < p->next))
            break; /* freed block at start or end of arena */
    
    decrementOccupiedMemory(headp->size);
    if (headp + headp->size == p->next) {
        headp->size += p->next->size;
        headp->next = p->next->next;
    } else {
        headp->next = p->next;
    }

    if (p + p->size == headp) {
        p->size += headp->size;
        p->next = headp->next;
    } else {
        p->next = headp;
    }
    
    freeList = p;
    return 0;
}
/*
int _memFree(void *blockp){
    MemHeader *headp, *auxp;

    headp = (MemHeader*)blockp - 1;
    for(auxp = freeList ; !(headp > auxp && headp < auxp->next) ; auxp = auxp->next)
        if(auxp >= auxp->next && (headp > auxp || headp < auxp->next))
            break;
    
    decrementOccupiedMemory(headp->size);
    
    // print("LIBERO: ");
    // printInt(headp->size, 10);
    // print("\n");

    if(headp + headp->size == auxp->next){
        headp->size += auxp->next->size;
        headp->next = headp->next->next;
    } else {
        headp->next = auxp->next;
    }
    if(auxp + auxp->size == headp){
        auxp->size += headp->size;
        auxp->next = headp->next;
    } else {
        auxp->next = headp;
    }
    freeList = auxp;
    return 0;
}
*/