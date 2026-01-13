#include "inc/mm.h"
#include "inc/kutils.h"
#include "inc/bvideo.h"
#include "inc/kernel.h"

WORD mmNextLoc = NULL;
WORD mmEndAddress = 0x1000000; //16MB
WORD mmStartAddress = 0;
mmList* mmMasterList = (mmList*)0xB00000;//0xA01000;
mmList* mmMasterListEnd;
/* <not so> Breif note.
   Originally the plan was for a list of lists, this proved annoying to code. Seriously, bad idea.
   Maybe once *everything* else works we can try one. kmalloc + kfree are basically wrappers so the
   whole kernel mm can be switched easily.

   Ended up with just 1 list. It may be prudent to use multiple lists in the future, the code will mostly work with such.
   It's late, I'm stressed... hence the type talk.

   *This entire mm system needs to be tested exhaustively. So far simple tests work.

   *feeing memory* - in porgress
   Currently a "freed" chunk of memory still has an entry in the list.
   This is to KISS. Once a peice of memory  is allocated, as far as the list is concerned it
   is neve un-allocated. It should be re-used promptly if space is adequate. This should cause excesisve
   fragmentation of memory. We either need to find a way to make this better, or perform cleanup on a regular basis.
   At the time of writing this comment, I am not sure which way is best or even doable.
*/


// Cannot Allocate Memory before calling this.
mmList* mmListInit(void) {
    memset(mmMasterList, 0, sizeof(mmList));
    mmMasterList->data.address = (WORD)mmMasterList;
    mmMasterList->data.size = 0x1000;
    mmMasterList->data.status = MM_STATUS_ALLOC | MM_STATUS_MM;
    mmNextLoc = (WORD)mmMasterList +mmMasterList->data.size;
    mmStartAddress = (WORD)mmMasterList;
    return  mmMasterList;
}

// finds the end of the list
mmList* mmListGetEnd(mmList* list) {
    while (list->next) {
        list = list->next;
    }
    mmMasterListEnd = list;
    return list;
}
// finds the begining of the list
mmList* mmListGetBeg(mmList* list) {
 /*   faster just to cast from var.
    while (list->prev) {
        list = list->prev;
    }
*/
    list = (mmList*)mmStartAddress;
    return list;
}

void mmListCopyLE(mmList* list, mmListEntry* le) {
    list->data.status = le->status;
    list->data.address = le->address;
    list->data.size = le->size;
}

WORD mmListFindFreeBlock(WORD size,WORD* nextloc) {
    mmList* tmp = mmListGetBeg(mmMasterList);
    DWORD address = 0;
    while ((tmp->next)) { // fix
        if ((tmp->data.status == MM_STATUS_FREE) && (tmp->data.size >= size)) {
            address = tmp->data.address;
            tmp->data.address += size;
            tmp->data.size -= size;
        }
        tmp = tmp->next;
    }
    if (!address) {
        if ((*nextloc + size) >=  mmEndAddress) {
            return NULL; // out of memeory
        }
        address = *nextloc;
        *nextloc +=size;
    }
    return address;
}

// Allocator for memory list  chunks/chips
mmList* mmListAlloc(mmList* list, WORD size) {
    if ((size +mmNextLoc) >= mmEndAddress) {
        return NULL;
    }
    list->next = (mmList*)mmListFindFreeBlock(size,&mmNextLoc);//nextloc?
    list->next->prev = list;
    list->next->next = NULL;
    list->next->data.address = (WORD)list->next;
    list->next->data.size = size;
    list->next->data.status = MM_STATUS_ALLOC | MM_STATUS_MM;
    list = list->next;
    return list;
}

mmList* mmListFindFreeList(mmList* list) {
    mmListGetBeg(list);
    while (list->next) { // fix
        if (list->data.status == MM_STATUS_FREE) {
            return list;
        }
        list = list->next;
    }
    // should be the end of the list here
    if (!((WORD)(list+1)%MM_LIST_CHUNK)) { // this should be space & time optimized
        list->next = mmListAlloc(list, MM_LIST_CHUNK);
        if (!list->next) {
            return NULL;
        }
    } else  {
        list->next = list+1;
        list->next->prev = list;
        list->next->next = NULL;
        list = list->next;
    }
    return list;
}
// first 8 bytes of address point to master list entry
void mmListBrandChunk(mmList* list) {
    *((BYTE*)list->data.address) = MM_MAGIC;

}

// the actuall allocation.
mmList* mmListAdd(mmList* list, WORD size) { // this is essentially kmalloc
    list = mmListFindFreeList(list);
    if (!list) {
        return NULL;
    }
    list->data.address = mmListFindFreeBlock(size,&mmNextLoc);
    list->data.size = size;
    list->data.status = MM_STATUS_ALLOC;
    if (!list->data.address) {
        return NULL;
    }
    mmListBrandChunk(list);
    return list;
}

// search list for pointer and mark it as free. does not erease (time > security atm)!
mmList* mmListFindAndMarkFree(mmList* list, void* ptr) { //This is essentially kfree
    // note this starts at the list you hand it, not the begining.
    while(list->data.address != (WORD)ptr) {
        if (list->next == NULL) {
            return NULL; // you lied! it's not dyn all!
        }
        list = list->next;
    }
    list->data.status = MM_STATUS_FREE;
    return list;
}

// malloc for kernel.
void* kmalloc(size_t size) {
    return (void*)(mmListAdd(mmMasterList, size)->data.address);
}
// free for kernel
void kfree(void* ptr) {
    mmListFindAndMarkFree(mmListGetBeg(mmMasterList), ptr);
    ptr =NULL;
}

// for debuging purposes, attempt to dynamically allocate memory
BOOL mmMemoryAllocationTest(void) {
    WORD* test1;
    WORD* test2;
    WORD* test0;
    int i;
    bv_printf("mmStartAddress: %x, mmNextloc: %x, mmList: %x, sizeof(mmList): %x\n",mmStartAddress, mmNextLoc, mmMasterList, sizeof(mmList));
    for (i = 0; i <10; i++) {
        test0 = kmalloc(sizeof(WORD));
        test1 = kmalloc(sizeof(WORD)*20);
        test2 = kmalloc(sizeof(WORD)*10);
        test1[0]+=i;
        test2[0]+=test1[0]+i;
        bv_printf("Memmory Allocation Test: %x : %x - %x : %x\n", test1[0], test1, test2[0], test2);
        kfree(test0);
        kfree(test2);
    }
    bv_puts("mmMasterList entries: \n");
    i = 0;
    while (mmMasterList->next) {
        i++;
        bv_printf("%x %s", mmMasterList, i%5 ? "\t" : "\n"  );
        mmMasterList = mmMasterList->next;
    }
    return TRUE;
}
