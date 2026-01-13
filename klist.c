#include "inc/klist.h"
#include "inc/mm.h"
/* Simple double linked list for early kernel use. utilizes kmalloc
    Do not use before memory is initialized. mmListInit() must be called prior to klist use.
*/

klist* kListCreate(void) {
    klist* tmp = (klist*)kmalloc(sizeof(klist));
    if (!tmp) {
        return NULL;
    }
    tmp->data = NULL;
    tmp->next = NULL;
    tmp->prev = NULL;
    return tmp;
}
BOOL kListIsEmpty(klist* list) {
    if ((list->next== NULL) && (list->prev ==NULL) && (list->data == NULL)) {
        return TRUE;
    }
    return FALSE;
}
BOOL kListIsEmptyMemorySafe(klist* list) {
    if ((list->next== NULL) && (list->prev ==NULL)) {
        return TRUE;
    }
    return FALSE;
}

klist* kListAddA(klist* list,void* data) {
    klist* nl = kListCreate();
    if (!nl) { return NULL; }
    nl->next = list->next;
    list->next->prev = nl;
    list->next = nl;
    nl->prev = list;
    nl->data = data;
    return nl;
}

// use kListRemove or kListRemoveMemorySafe insetad
klist* kListRemove_internal(klist* list, BOOL kill) {
    void* data = list->data;
    klist* tmp = NULL;
    if (list->prev) {
        tmp = list->prev;
    } else if (list->next) {
        tmp = list->next;
    }
    list->prev->next = list->next;
    list->next->prev = list->prev;
    kfree(list);
    if (kill) { kfree(data); }
    return tmp;
}
// These might make better macros
klist* kListRemove(klist* list) {
    return kListRemove_internal(list, TRUE);
}

klist* kListRemoveMemorySafe(klist* list) {
    return kListRemove_internal(list, FALSE);
}
klist* kListGetStart(klist* list) {
    while (list->prev) {
        list = list->prev;
    }
    return list;
}
klist* kListGetEnd(klist* list) {
    while (list->next) {
        list = list->next;
    }
    return list;
}

void kListEmpty_internal(klist* list, BOOL kill) {
    list = kListGetStart(list);
    while (!kListIsEmptyMemorySafe(list)) {
        list = kListRemove_internal(list, kill);
    }
}
