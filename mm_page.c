// Memory Managment page setup & funcs
#include "inc/gdt.h" // has some initial memory info
#include "inc/x86_asm/x86msr.h"
#include "inc/kutils.h"
#include "inc/bvideo.h"
DWORD mm_page_PDPTE[4]__attribute__((aligned(0x20)));
DWORD mm_page_dir[4][512] __attribute__((aligned(0x01000)));
DWORD mm_page_table[4][512][512]__attribute__((aligned(0x01000)));// probably dynamically allocate the rest

// Currently pages entire 4GB ring0, need to set some to ring 3
void mm_page_init_pages(void) {
    int i,j, k;
    memset(&mm_page_PDPTE, 0, sizeof(DWORD)*4);
    memset(&mm_page_dir, 0, sizeof(DWORD)*512*4);
    memset(&mm_page_table, 0, sizeof(DWORD)*512*512*4);
    for (i = 0; i<4; i++) {
        mm_page_PDPTE[i] = (WORD)&mm_page_dir[i][0];
        mm_page_PDPTE[i] |=1;
    }
    DWORD address = 0;
    // we need to put a good deal of this in ring3
    for (k = 0; k <4; k++) {
        for (i = 0; i <512; i++) {
            mm_page_dir[k][i] = (WORD)&mm_page_table[k][i][0];
            mm_page_dir[k][i] |= 3;
            for (j = 0; j<512; j++) {
                mm_page_table[k][i][j] = address;
                mm_page_table[k][i][j] |= 3;
                address += 0x1000;
            }
        }
    }
}

void mm_page_init(void) {
    mm_page_init_pages();
    asm ("xchg %bx, %bx");
    asm volatile ("movl %cr4, %eax; bts $5, %eax; movl %eax, %cr4 ");
    asm volatile ("movl %%eax, %%cr3" :: "a" (&mm_page_PDPTE));
    asm("xchg %bx, %bx");
    asm volatile ("movl %cr0, %eax; orl $0x80000000, %eax; movl %eax, %cr0");
    asm ("xchg %bx, %bx");
}
// find the page at address. Should b 4kb aligned. useful for changing caching & whatnot return 0 on fail.
DWORD* mm_page_fk_table(WORD address) { //find kernel.
    if (address%4) // not 4kb aligned, don't bother
        return 0;
    int i,j, k;
    for (k = 0; k <4; k++) {
        for (i = 0; i <512; i++) {
            for (j=0; j<512; j++) {// should be 8, testing...
                if ((mm_page_table[k][i][j] & 0x3FFFFFFFFFFFF000) ==address ) //and out the stuff we don't want
                    return &mm_page_table[k][i][j];
            }
        }
    }
    return 0;
}
// Set a page to uncachable. Should be ok on P3 and up. return 0 on fail.
WORD mm_page_set_unchachable(WORD address) {
    DWORD* _page = mm_page_fk_table(address);
    if (!_page)
        return 0; // probably we sent something that was not on a page boundary.
    *_page &= 0xFFFFFFFFFFFFFF67; //clear the PAT PWD PCD
    *_page |= 0x18; // set bits 4,3 to 1 - should correspond to PATENTRY3, which is assumed UC. it is the default
    invalidate_tlb(address); //flush the tlb for the page.
    return 1;
}
