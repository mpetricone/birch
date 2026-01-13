#ifndef _MM_
#define _MM_
#include "types.h"


#define MM_STATUS_FREE 0
#define MM_STATUS_ALLOC 1
#define MM_STATUS_EMPTY 2
#define MM_STATUS_MM 16;

#define MM_LIST_CHUNK 4096
#define MM_LIST_CHIP 32

#define MM_MAGIC 0xED

typedef struct mmListEntry_t {
    BYTE status;
    WORD address;
    WORD size;
} mmListEntry;


typedef struct mmList_t {
    struct mmList_t* prev;
    struct mmList_t* next;
    mmListEntry data;
} mmList;

// Memory Managment Header
extern void mm_page_init(void);
extern WORD mm_page_set_unchachable(WORD address);
extern void* kmalloc(size_t size);
extern void kfree(void* ptr);
extern mmList* mmListInit(void);
extern BOOL mmMemoryAllocationTest(void);
#endif
