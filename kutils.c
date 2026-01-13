#include "types.h"

void memcpy(void* dest, const void* src,UINT size) {
    int i;
    for (i = 0; i<size; i++) {
        ((BYTE*)dest)[i]=((BYTE*)src)[i];
    }
}

void memset(void* data, BYTE info,UINT size) {
    int i;
    for (i = 0; i<size; i++) {
        ((BYTE*)data)[i]=info;
    }
}
