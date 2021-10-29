#include <stdint.h>
#include <stddef.h>
#include <lib.h>
#include <console.h>

/******************************************************************************
 *
 * Definitions
 *
 ******************************************************************************/
#define MAX_ORDER       28
#define MIN_ORDER       4   // 2 ** 4 == 16 bytes

/* the order ranges 0->->MAX_ORDER, the largest memblockp is 2**(MAX_ORDER) */
#define POOLSIZE        (1 << MAX_ORDER)

/* blockps are of size 2**i-> */
#define BLOCKSIZE(i)    (1 << (i))

/* the address of the buddy of a blockp from freelists[i]-> */
#define _MEMBASE        ((uintptr_t)BUDDY->pool)
#define _OFFSET(b)      ((uintptr_t)b - _MEMBASE)
#define _BUDDYOF(b, i)  (_OFFSET(b) ^ (1 << (i)))
#define BUDDYOF(b, i)   ((void*)( _BUDDYOF(b, i) + _MEMBASE))

// not used yet, for higher order memory alignment
#define ROUND4(x)       ((x % 4) ? (x / 4 + 1) * 4 : x)

/******************************************************************************
 *
 * Types & Globals
 *
 ******************************************************************************/

typedef struct buddy {
  void* freelist[MAX_ORDER + 2];  // one more slot for first blockp in pool
  uint8_t pool[POOLSIZE];
} buddy_t;

static buddy_t* BUDDY = 0x900000;
static int wasInitialized = 0;

void buddy_init() {
    memset(BUDDY, 0, sizeof(buddy_t));
    BUDDY->freelist[MAX_ORDER] = BUDDY->pool;
}

void* memAlloc(size_t size, int options) {
    if(wasInitialized == 0){
        buddy_init();
        wasInitialized = 1;
    }
    int i, order;
    void *blockp, *buddy;

    // calculate minimal order for this size
    i = 0;
    while (BLOCKSIZE(i) < size + 1) // one more byte for storing order
        i++;

    order = i = (i < MIN_ORDER) ? MIN_ORDER : i;

    // level up until non-null list found
    for (;; i++) {
        if (i > MAX_ORDER)
            return NULL;
        if (BUDDY->freelist[i])
            break;
    }

    // remove the blockp out of list
    blockp = BUDDY->freelist[i];
    BUDDY->freelist[i] = *(void**) BUDDY->freelist[i];

    // split until i == order
    while (i-- > order) {
        buddy = BUDDYOF(blockp, i);
        BUDDY->freelist[i] = buddy;
    }

    // store order in previous byte
    *((uint8_t*) (blockp - 1)) = order;
    return blockp;
}

int memFree(void* blockp) {
    int i;
    void* buddy;
    void* * p;

    // fetch order in previous byte
    i = *((uint8_t*) (blockp - 1));

    for (;; i++) {
        // calculate buddy
        buddy = BUDDYOF(blockp, i);
        p = &(BUDDY->freelist[i]);

        // find buddy in list
        while ((*p != NULL) && (*p != buddy)){
            p = (void* *) *p;
        }

        // not found, insert into list
        if (*p != buddy) {
            *(void**) blockp = BUDDY->freelist[i];
            BUDDY->freelist[i] = blockp;
            return 0;
        }
        // found, merged blockp starts from the lower one
        blockp = (blockp < buddy) ? blockp : buddy;
        // remove buddy out of list
        *p = *(void**) *p;
    }
    return 0;
}

void getMemInfo(MemoryInfo* meminfo){
    return NULL;
}

/*
 * The following functions are for simple tests->
 */

/*
static int count_blockps(int i) {

  int count = 0;
  void* * p = &(BUDDY->freelist[i]);

  while (*p != NULL) {
    count++;
    p = (void**) *p;
  }
  return count;
}

static int total_free() {

  int i, bytecount = 0;

  for (i = 0; i <= MAX_ORDER; i++) {
    bytecount += count_blockps(i) * BLOCKSIZE(i);
  }
  return bytecount;
}

static void print_list(int i) {

  printf("freelist[%d]: \n", i);

  void* *p = &BUDDY->freelist[i];
  while (*p != NULL) {
    printf("    0x%08lx, 0x%08lx\n", (uintptr_t) *p, (uintptr_t) *p - (uintptr_t) BUDDY->pool);
    p = (void**) *p;
  }
}

void print_buddy() {

  int i;

  printf("========================================\n");
  printf("MEMPOOL size: %d\n", POOLSIZE);
  printf("MEMPOOL start @ 0x%08x\n", (unsigned int) (uintptr_t) BUDDY->pool);
  printf("total free: %d\n", total_free());

  for (i = 0; i <= MAX_ORDER; i++) {
    print_list(i);
  }
}
*/