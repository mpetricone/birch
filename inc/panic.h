#ifndef _PANIC_
#define _PANIC_
#include "types.h"
#include "idt.h"

#define PANIC_HARDWARE_NOTSUPPORTED "\nThis hardware cannot run Birch."
#define PANIC_BIOS_NOTSUPPORTED "\nFailed to probe BIOS info, cannot run Birch."
// reserved 0-256 for interupt panics
// just use interrupt number for 0-255, 256 is wtf we don't have one!
#define PANIC_INTERRUPT_UNHANDLED 256
#define PANIC_BOOT_LOWRAM 0x3E8 // INFO and func should be passed as below
    #define PANIC_BOOT_LOWRAM_INFO 0x4C4F570 //"LOW" we don't actually care about where we are, Birch doesn't care to run.
    #define PANIC_BOOT_LOWRAM_FUNC 0x52414D0 //"RAM"
extern void panic(WORD panic_code,WORD info, WORD func,isr_info* interrupt);
extern void panic_boot(char* fail_text);
#endif
