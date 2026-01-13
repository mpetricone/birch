// IO APIC setup & functions
#include "inc/types.h"
#include "inc/apic.h"
#include "inc/bvideo.h"
volatile void* ioapic_loc = (WORD*)IOAPIC_BASE;

void ioapic_write(WORD reg, WORD value) {
    *(WORD*)ioapic_loc = reg;
    *(WORD*)(ioapic_loc+0x10) = value;
}

volatile WORD ioapic_read(WORD reg) {
    *(WORD*)ioapic_loc = reg;
    return *(WORD*)(ioapic_loc+0x10);
}

WORD ioapic_getversion(void) {
    return ioapic_read(IOAPIC_REG_VERSION);
}

WORD ioapic_getid(void) {
    return ioapic_read(IOAPIC_REG_ID);
}
