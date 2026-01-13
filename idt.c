#include "inc/idt.h"
#include "inc/bvideo.h"
#include "inc/panic.h"
// IDT functions and vars
idt idt_list[IDT_TOTAL];
idt_ptr idt_pointer;

// initializes all the idt structs to empty routines
void idt_ready(void) {
    int i;
    for (i = 0; i<IDT_TOTAL; i++) {
        idt_list[i].offset_low = 0;
        idt_list[i].selector = 0;
        idt_list[i].type_access = IDT_RING0_INTERUPT_ACCESS;
        idt_list[i].offset_high= 0;
    }
}
// sets an individual idt entry, pass by pointer
void idt_set_isr(idt* interrupt, USHORT segment, DWORD offset, USHORT access){
    interrupt->offset_low = (offset&0xFFFF);
    interrupt->selector = segment;
    interrupt->offset_high =(offset>>16)&0xFFFF;
}
// sets the isrs we need, see inc/idt.h for macros
void idt_set_system_isr(void) {
    ISR_SET_RING0(IDT_GET_DIVIDEBYZEO, isr_div)
    ISR_SET_RING0(IDT_GET_RESERVED1, isr_res1)
    ISR_SET_RING0(IDT_GET_NMI, isr_nmi)
    ISR_SET_RING0(IDT_GET_BREAKPOINT, isr_breakpoint)
    ISR_SET_RING0(IDT_GET_OVERFLOW, isr_overflow)
    ISR_SET_RING0(IDT_GET_BOUND, isr_bound)
    ISR_SET_RING0(IDT_GET_INVALIDOPCODE, isr_invopc)
    ISR_SET_RING0(IDT_GET_NOMATHCOPROC, isr_dna_nmc)
    ISR_SET_RING0(IDT_GET_DOUBLEFAULT, isr_df)
    ISR_SET_RING0(IDT_GET_ARCHAIC_FLOAT, isr_cso)
    ISR_SET_RING0(IDT_GET_INVALID_TSS, isr_invtss)
    ISR_SET_RING0(IDT_GET_SEGMENTNOTHERE, isr_snp)
    ISR_SET_RING0(IDT_GET_STACKSEGFAULT, isr_ssf)
    ISR_SET_RING0(IDT_GET_GPF, isr_gp)
    ISR_SET_RING0(IDT_GET_PAGEFAULT, isr_pf)
    ISR_SET_RING0(IDT_GET_RESERVED2, isr_res2)
    ISR_SET_RING0(IDT_GET_X87FLOATERROR, isr_fpe)
    ISR_SET_RING0(IDT_GET_ALIGNCHECK, isr_ac)
    ISR_SET_RING0(IDT_GET_MACHINECHECK, isr_mc)
    ISR_SET_RING0(IDT_GET_SIMD, isr_sse)
}
// runs idt_init and installs the full 256 idt entries
void idt_init(void) {
    // first the pointer
    idt_pointer.limit = (sizeof(idt)*IDT_TOTAL)-1;
    idt_pointer.base = (WORD)&idt_list;
    // now set all the idt for now all ring 0, this will change.
    idt_ready();
    idt_load(); //asm call
    idt_set_system_isr();
}

void isr_chandler(isr_info* info) {
    switch (info->int_num) {
        // start of intel used ints - need to be written.
        case 0: {
            bv_puts("Divide by Zero fault");
            IDT_ISR_PANIC
        } break;
        case 1: {
            IDT_ISR_PANIC
        } break;
        case 2: {
            IDT_ISR_PANIC
        } break;
        case 3: {
            IDT_ISR_PANIC
        } break;
        case 4: {
            IDT_ISR_PANIC
        } break;
        case 5: {
            IDT_ISR_PANIC
        } break;
        case 6: {
            IDT_ISR_PANIC
        } break;
        case 7: {
            IDT_ISR_PANIC
        } break;
        case 8: {
            IDT_ISR_PANIC
        } break;
        case 9: {
            IDT_ISR_PANIC
        } break;
        case 10: {
            IDT_ISR_PANIC
        } break;
        case 11: {
            IDT_ISR_PANIC
        } break;
        case 12: {
            IDT_ISR_PANIC
        } break;
        case 13: {
            IDT_ISR_PANIC
        } break;
        case 14: {
            IDT_ISR_PANIC
        } break;
        case 15: {
            IDT_ISR_PANIC
        } break;
        case 16: {
            IDT_ISR_PANIC
        } break;
        case 17: {
            IDT_ISR_PANIC
        } break;
        case 18: {
            IDT_ISR_PANIC
        } break;
        case 19: {
            IDT_ISR_PANIC
        } break;
        // end of intel used
        default: {
            panic(PANIC_INTERRUPT_UNHANDLED,(WORD)info, (WORD)isr_chandler,info);
        }
    }
}
