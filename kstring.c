#include "types.h"
#include "kutils.h"
#include "bvideo.h"
char* kstrrev(char* str, int len) {
    char *start = str, *end = str +len -1;
    while (start < end) {
        char tmp =*start;
        *start++=*end;
        *end-- =tmp;
    }
    return str;
}

UINT kstrlen(char* str) {
    int i = 0;
    while (str[i] != 0) {
    i++;
    }
    return i-1;
}
char* kstrpl(char* out,char rep, char c) {
    while (*out!=0) {
        if (*out==rep)
            *out=c;
        out++;
    }
    return out;
}
// decided probably don't care about signs, mostly want memory addresses
char* kitoa(unsigned int num, int base, char* out, int osize) {
    char out_array[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
    memset(out, '0', osize);
    int i = 0;
    while (num>0) {
        out[i++] = out_array[num%base];
        num /= base;
    }
    out[i]=0;
    kstrrev(out, i);// for 0 val we don't need to strrev
    return out;
}
//
char* kitoa_zeros(unsigned int num, char* out, int osize) {
    char out_array[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (osize < 8) {
        return NULL;
    }
    int i = 7;
    out[8] = 0;
    while (i >=0) {
        out[i--] = out_array[num%16];
        num /=16;
    }
    return out;
}

