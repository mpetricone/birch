#ifndef _IDT_
#define _IDT_
#include "types.h"
// IDT stucts and support

// technically the same as a gdt, but this feels right
typedef struct _idt {
    USHORT offset_low;
    USHORT selector; // not always a segment so...
    USHORT type_access; // why?
    USHORT offset_high;

} __attribute__((packed)) idt;
// Some macros for the acess and type bits of the idt entry
//These might be off by 1 bit - 5 bits reserved?
#define IDT_RING0_INTERUPT_ACCESS 0x8E00
#define IDT_RING3_INTERUPT_ACCESS 0xEE00
//These should be fine
#define IDT_RING0_TASK_ACCESS     0x8500
#define IDT_RING3_TASK_ACCESS     0xE500
//Also might be off by 1 bit
#define IDT_RING0_TRAP_ACCESS     0x8F00
#define IDT_RING3_TRAP_ACCESS     0xEF00

typedef struct _idt_pointer {
    USHORT limit;
    WORD base;
} __attribute__((packed)) idt_ptr;
#define IDT_TOTAL 256
// Brians tutoral is teaching me asm.. this struct is nifty. beleive
typedef struct _isr_info {
    WORD gs, fs, es, ds; //
    WORD edi, esi, edp, esp, ebx, edx, ecx, eax; //pusha
    WORD int_num, error_code; // interupt func handles this.
    WORD eip, cs, eflags, useresp, ss; //interrupt automatically pushes
}  isr_info;
extern idt idt_list[IDT_TOTAL];
extern idt_ptr idt_pointer;
// asm include
extern void idt_load(void);
// asm interupt handlers
extern void isr_div(void); //divide by zero(void);
extern void idt_load(void);
extern void isr_div(void);
extern void isr_res1(void);
extern void isr_nmi(void);
extern void isr_breakpoint(void);
extern void isr_overflow(void);
extern void isr_bound(void);
extern void isr_invopc(void);
extern void isr_dna_nmc(void);
extern void isr_df(void);
extern void isr_cso(void);
extern void isr_invtss(void);
extern void isr_snp(void);
extern void isr_ssf(void);
extern void isr_gp(void);
extern void isr_pf(void);
extern void isr_res2(void);
extern void isr_fpe(void);
extern void isr_ac(void);
extern void isr_mc(void);
extern void isr_sse(void);
// idt prototypes
extern void idt_init(void);
extern void isr_chandler(isr_info* info);
// Some macros for the system idts - considering seperate handlers for each.
#define ISR_SET_RING0(i, f)     idt_set_isr(&i, 0x08,(WORD) f, IDT_RING0_INTERUPT_ACCESS);
#define IDT_GET_DIVIDEBYZEO     idt_list[0]
#define IDT_GET_RESERVED1       idt_list[1]
#define IDT_GET_NMI             idt_list[2]
#define IDT_GET_BREAKPOINT      idt_list[3]
#define IDT_GET_OVERFLOW        idt_list[4]
#define IDT_GET_BOUND           idt_list[5]
#define IDT_GET_INVALIDOPCODE   idt_list[6]
#define IDT_GET_NOMATHCOPROC    idt_list[7]
#define IDT_GET_DOUBLEFAULT     idt_list[8]
#define IDT_GET_ARCHAIC_FLOAT   idt_list[9]
#define IDT_GET_INVALID_TSS     idt_list[10]
#define IDT_GET_SEGMENTNOTHERE  idt_list[11]
#define IDT_GET_STACKSEGFAULT   idt_list[12]
#define IDT_GET_GPF             idt_list[13]
#define IDT_GET_PAGEFAULT       idt_list[14]
#define IDT_GET_RESERVED2       idt_list[15]
#define IDT_GET_X87FLOATERROR   idt_list[16]
#define IDT_GET_ALIGNCHECK      idt_list[17]
#define IDT_GET_MACHINECHECK    idt_list[18]
#define IDT_GET_SIMD            idt_list[19]

// panic helper
#define IDT_ISR_PANIC panic((WORD)info->int_num, (WORD)&info, (WORD)info->eip, info);
#endif
