#include "inc/gdt.h"
#include "inc/bvideo.h"
#include "inc/kutils.h"
#include "inc/panic.h"
// GDT setup... i already hate these things. derived from http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial
//NOTE: this file is compiled as cgdt.o, and gdt.asm will be compiled as agdt.o
gdt_ptr gdt_pointer;
gdt_ptr gdt_pcheck;
gdt gdt_list[GDT_TOTAL];
mb_mem_descriptor mm_usable[2];

// Get some info on ram, to help set up usefull GDT and eventually paging too.
// this is what we really want to use
void gdt_inspect_mm(mb_mm* memory) {
    mb_mem_descriptor* desc;
    UINT high_len = 0;
    for (desc = (mb_mem_descriptor*)memory->address ;
        (UINT)desc < (memory->address+memory->length);
        desc= (mb_mem_descriptor*)((UINT)desc+desc->size +sizeof(desc->size))) {
        if (desc->type == 1) {
            if (desc->length >high_len && desc->length>0x1000000) {
                mm_usable[0].length = 0x1000000-desc->base_address;
                mm_usable[0].base_address = desc->base_address;
                mm_usable[0].type = 1;
                mm_usable[1].length = desc->length-0x1000000;
                mm_usable[1].base_address = 0x1000000;
                mm_usable[1].type = 1;
                high_len = desc->length;
            }
        }
        bv_printf("%z:%z:-:-:%z:%z::%x\n",(unsigned)(desc->base_address>>32),(unsigned)(desc->base_address&0xFFFFFFFF),(unsigned)(desc->length>>32), (unsigned)(desc->length&0xFFFFFFFF), desc->type);
    }
    if (!mm_usable[0].length && !mm_usable[1].length) {
        panic(PANIC_BOOT_LOWRAM, PANIC_BOOT_LOWRAM_INFO, PANIC_BOOT_LOWRAM_FUNC, NULL);
    }
    bv_printf("KERNEL RAM: %dMB\n", (mm_usable[0].length) >>20);
    bv_printf("SYSTEM RAM: %dMB\n", (mm_usable[1].length) >>20);

}
// this is our fallback, make sure it's got a valid number, or something will eventually go wrong.
void gdt_declare_mm(WORD memory_high) {
    // memory_high is in kb.
    // to be done.
}
// Thanks be to brian, cause this is just not fun to figure out when your brains not in hex mode.
void gdt_set(gdt* op,ULONG base, ULONG limit, BYTE access, BYTE gran ) {
    op->base_low = (base &0xFFFF);
    op->base_mid = (base >>16) & 0xFF;
    op->base_high = (base >>24) &0xFF;

    op->limit_low = (limit &0xFFFF);
    op->granularity = (limit>>16) &0x0F;

    op->granularity |= (gran&0xF0);
    op->access = access;
}
// install the gdts... which still make no fucking sense to me.
void gdt_init(void) {
    gdt_pointer.limit = (sizeof(gdt)*GDT_TOTAL)-1;
    gdt_pointer.base = (UINT)&gdt_list;
    // obvisouly, null
    gdt_set(&GDT_GET_NULL, 0, 0, 0, 0);
    gdt_set(&GDT_GET_CODE, 0, 0xFFFFFF, 0x9A, 0xCF ); // first 16 megs is kernels. - note -1 from the num or we tripplef
    gdt_set(&GDT_GET_DATA, 0, 0xFFFFFF, 0x92, 0xCF);
    gdt_set(&GDT_GET_RING3_CODE, 0xFFFFFF,0xFFFFFFFF, 0xFA,0xCF); // the rest is ring3 land
    gdt_set(&GDT_GET_RING3_DATA, 0xFFFFFF,0xFFFFFFFF, 0xF2,0xCF);
    gdt_install();//asm function call
    // This was helping to debug, i rather like it. I was checking to make sure values were being passed to asm right, turned out I was shor 1 BYTE
   // bv_printf("%x != %x = %x != %x = %x", gdt_pcheck.base, gdt_pointer.base, &gdt_list, gdt_pcheck.limit, gdt_pointer.limit);
}
// Can, you can tell I hate this file?
