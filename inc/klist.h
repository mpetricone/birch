#ifndef _KLIST_
#define _KLIST_
/* Simple double linked list for early kernel use. utilizes kmalloc
    Do not use before memory is initialized. mmListInit() must be called prior to klist use.
*/

typedef struct klist_t {
    struct klist_t* next;
    struct klist_t* prev;
    void* data;
} klist;
#endif // _KLIST_
