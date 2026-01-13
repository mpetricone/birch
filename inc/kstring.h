#ifndef _KSTRING_
#define _KSTRING_
extern char* kitoa(unsigned int num, int base, char* out, int osize);
extern char* kitoa_zeros(unsigned int num, char* out, int osize) ;
extern UINT kstrlen(char* str);
#endif
