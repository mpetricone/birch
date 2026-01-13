#ifndef _GDT_
#define _GDT_
#include "types.h"
#include "mb.h"
// This is going to be hell
// minum amount of ram, or we should panic
#define MM_MIN_RAM 0x4000
typedef struct _gtd {
    USHORT      limit_low;
    USHORT      base_low;
    BYTE        base_mid;
    BYTE        access; //low3 bits = seg type, highbit= descriptor type (0=system,1=code or data)
    BYTE        granularity; //gotta be a better way.
    BYTE        base_high;
} __attribute__((__packed__)) gdt;

typedef struct _gdt_ptr {
    USHORT limit;// the death of me, i had this as a BYTE for a day, triple fault hell.
    UINT base;
} __attribute__((__packed__)) gdt_ptr;
#define GDT_TOTAL 5
extern gdt gdt_list[GDT_TOTAL]; //we will probably end up with more.
extern gdt_ptr gdt_pointer;
extern gdt_ptr gdt_pcheck;
// asm function to install and flush gdt
extern void gdt_install(void);
extern void gdt_init(void);
extern void gdt_inspect_mm(mb_mm* memory);
extern void gdt_declare_mm(WORD memory_high);
// just some macros so we dont' have to remember what the offsets are
#define GDT_GET_NULL gdt_list[0]
#define GDT_GET_CODE gdt_list[1]
#define GDT_GET_DATA gdt_list[2]
#define GDT_GET_RING3_CODE gdt_list[3]
#define GDT_GET_RING3_DATA gdt_list[4]
#endif
