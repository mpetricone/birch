//APIC programming & utils
/*
    we're trying to map the apic to this memory, to move it away from the default, which
    could seriously hinder mmp in the future. I am not sure doing it like this will work.
    It should be dynamically mapped to a page in the future.
*/
#include "inc/mm.h"
#include "inc/bvideo.h"
#include "inc/x86_asm/x86msr.h"
#include "inc/kutils.h"
#include "inc/apic.h"
volatile void* apic_loc =(void*) APIC_BASE;//  the move wasn't liked x300000;
// what kind of apic are we?
volatile int apic_enable_move() {
    WORD low, high;
    rdmsr(MSR_IA32_APIC_BASE,low, high); // get the msr state.
    if (low &0x800) {
        bv_puts("Local APIC active.");
    } else {
        bv_puts("Activating Local APIC.");
        low |=0x800; // turn apic on globally.
    }
    low &= 0xFFFF; // clear the current address, 4kb aligned.
    low |= (WORD)apic_loc; // set the base to apic_loc
    wrmsr(MSR_IA32_APIC_BASE, low, high); // put it back. This should turn the local APIC on if it is not.
    // check to make sure it took.
    rdmsr(MSR_IA32_APIC_BASE, low, high);
    if ((low &0xFFFFF000) !=(WORD)apic_loc) {
        bv_printf("\nLocal APIC map not moved. %x, %x", (low&0xFFFFF000), apic_loc); // might help to figure out what's wrong.
        return 0;
    }
    return 1; // bootsrap APIC has been moved and is global enabled.
}

volatile WORD apic_write(WORD value, WORD reg) {
    WORD oldval = apic_read(reg);
    *((WORD*)(apic_loc+reg)) = value;
    return oldval;
}
// OR=
volatile WORD apic_owrite(WORD value, WORD reg) {
    WORD oldval = apic_read(reg);
    *((WORD*)(apic_loc+reg)) |= value;
    return oldval;
}
volatile WORD apic_read(WORD reg) {
    return *((WORD*)(apic_loc+reg));
}

// software enable via spurious
volatile WORD apic_enable(void) {
    return apic_owrite(0x800,APIC_REG_SPURIOUS);
}
// software disable via spurious
volatile WORD apic_disable(void) {
    WORD spur = apic_read(APIC_REG_SPURIOUS);
    return apic_write(spur^0x800, APIC_REG_SPURIOUS);
}

int apic_init(void) {
    if (!mm_page_set_unchachable((WORD)apic_loc)) // set page as uncachable.
        return 0; //failed, the APIC won't work right at this loc(INTEL docs).
    if (!apic_enable_move())
        return 0;
    // at this point the bootstrap APIC is located at apic_loc and global enabled. or we've bailed.
    // we still need to check mr spurious bit 8 to see if software enable is go.
    bv_printf("\nApic ID:Version Info %z:%z", apic_read(APIC_REG_ID), apic_read(APIC_REG_VERSION));
    apic_enable();
    apic_write(0x10000&APIC_INTERRUPT_TIMER, APIC_REG_TIMER);
    apic_write(0x8000&APIC_INTERRUPT_THERMAL, APIC_REG_THERMAL);
    apic_write(0x8000&APIC_INTERRUPT_PERFCNT, APIC_REG_PERFCNT);
    apic_write(APIC_INTERRUPT_LINT0, APIC_REG_LINT0);
    apic_write(APIC_INTERRUPT_LINT1, APIC_REG_LINT1);
    apic_write(APIC_INTERRUPT_LVTERROR, APIC_REG_LVTERROR);
   // apic_write(todo, APIC_INTERRUPT_LVT_CMCI);
    return 1;
}
