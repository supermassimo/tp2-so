#include <memManager.h>

#define PAGE_SIZE 0x1000
#define MEM_LIMIT 536870912             // 512MB

typedef struct MemHeader {
    struct MemHeader* next;
    size_t size;
} MemHeader;

static MemHeader baseHeader;
static MemHeader* freeList = NULL;
static uint8_t* nextToMap= (uint8_t*)0x900000;

void* memMap(size_t size){
    if((size_t)nextToMap + size > MEM_LIMIT)
        return NULL;
    void* blockp = nextToMap;
    nextToMap += size;
    return blockp;
}

static void applyOptions(MemHeader *memBlock, int options){
    switch(options){
        case 1:
            memset(memBlock+1, 0, memBlock->size);
            break;
        default:
            break;
    }
}

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
    return freeList;        // returns block added to free list
}

void* memAlloc(size_t size, int options){
    MemHeader *auxp, *currentBlock;
    size_t headerBlocks = (size + sizeof(MemHeader) - 1)/sizeof(MemHeader) + 1;         // Always round up # of block needed

    if((currentBlock = freeList) == NULL){     /* No free blocks to asign yet*/
        baseHeader.next = freeList = currentBlock = &baseHeader;
        baseHeader.size = 0;
    }

    for(auxp = currentBlock->next ;  ; currentBlock = auxp, auxp = auxp->next){
        if(auxp->size >= headerBlocks){
            if(auxp->size == headerBlocks)
                currentBlock->next = auxp->next;
            else {
                auxp->size -= headerBlocks;
                auxp += auxp->size;
                auxp->size = headerBlocks;
            }
            freeList = currentBlock;
            applyOptions(auxp, options);
            return (void*)(auxp+1);
        }
        if(auxp == freeList)
            if((auxp = getBlocks(headerBlocks)) == NULL)
                return NULL;                        // If system has no more free memory, return NULL
    }
}

int memFree(void *blockp){
    MemHeader *headp, *auxp;

    headp = (MemHeader*)blockp - 1;
    for(auxp = freeList ; !(headp > auxp && headp < auxp->next) ; auxp = auxp->next)
        if(auxp >= auxp->next && (headp > auxp || headp < auxp->next))
            break;
    
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