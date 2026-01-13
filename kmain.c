#include "inc/kernel.h"
#include "inc/mb.h"
#include "inc/bvideo.h"
#include "inc/kstring.h"
#include "inc/kutils.h"
#include "inc/gdt.h"
#include "inc/idt.h"
#include "inc/panic.h"
#include "inc/mm.h"
#include "inc/apic.h"
#include "inc/bios.h"
void kboot_banner(void) {
    bv_cls();
    bv_setcolor(0x2f);
    bv_puts(KERNEL_BOOT_HEADER);// The green Birch
    bv_setcolor(0x07);
}

int kinit(mb_info* boot_info) {
    bv_init();
    kboot_banner();
    if (!cpu_check()) {
        panic_boot(PANIC_HARDWARE_NOTSUPPORTED); // hardware is old, say so and stop.
    }
    bv_printf("Memory, %d:%d",boot_info->memory.mem_lower,boot_info->memory.mem_upper );
    BOOT_CONSOLE_AOK_EXT("Multiboot")
    if (!(boot_info->flags &0x20)) { // is there a memory map?
        bv_puts("\nMissing Memory Map. Guessing...\n");
        if (boot_info->memory.mem_upper < MM_MIN_RAM) {
            // either have too little ram, or Multiboot can't determine ram the way we need. Not planning on probing.
            panic(PANIC_BOOT_LOWRAM, PANIC_BOOT_LOWRAM_INFO, PANIC_BOOT_LOWRAM_FUNC, NULL);
        }
        gdt_declare_mm(boot_info->memory.mem_upper); // try to use this, it seems to be returning somewhat usable values.
        BOOT_CONSOLE_PASSABLE // or so we think, surely not AOK
    } else { // this is where we would like to go.
        gdt_inspect_mm(&boot_info->memory_map);
        bv_puts("Memory Map:");
        BOOT_CONSOLE_AOK
        gdt_init();//first really important thing we do, install gdt
        bv_puts("GDT:");
        BOOT_CONSOLE_AOK
    }
    idt_init();
    bv_puts("Installing IDT");
    BOOT_CONSOLE_AOK
    bv_puts("Enable Paging, kenel space map.");
    mm_page_init();
    BOOT_CONSOLE_AOK
    bv_puts("Programming APIC...");
    if (!apic_init()) {
        bv_puts("\tFAILED. Probably a page cache issue.\n");
    } else {
        BOOT_CONSOLE_AOK
    }
    //ready dynamic memory allocation
    kboot_banner(); // just to clean screen
    mmListInit();
    #ifdef DEBUG_MEMORY // added 5/23/2011, seems to work ok, and it's cluttering
    mmMemoryAllocationTest();
    #endif //DEBUG_MEMORY
    // Not sure if this is wrong, or there are no MB tables
    bv_puts("Probing BIOS for MP info...\n");
    if (!bios_probe()) { // no MP table, so we'll try guessing an ioapic
        BOOT_CONSOLE_FAIL
        bv_printf("IOAPIC ID:Version: %x:%x",ioapic_getid(),ioapic_getversion());
        BOOT_CONSOLE_PASSALBE_EXT("IOAPIC");
    } else {
        BOOT_CONSOLE_AOK
    }

    return 1;
}

int kmain(mb_info* boot_info, unsigned int magic) {
    if (magic !=0x2BADB002) {
        bv_puts("Multiboot Failure.\n");
        return 0;// Multiboot Fail.
    }
    return kinit(boot_info);
}
