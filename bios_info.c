// Used to get bios info on boot
#include "inc/bios.h"
#include "inc/bvideo.h"
#include "inc/kutils.h"
/* I haven't been able to find an MPFP on Bochs, Qemu or VirtualBox, but the MPCT is there.
 * Wondering if something is wrong with this logic, as I thought the MPFP had to be around.
*/
// parse the bios mp info
int bios_parse_mpct(WORD* mpct_loc) {
    MPCT *mpct = (MPCT*)mpct_loc;
    bv_printf("BIOS MPCT at %z", mpct_loc);
    if (mpct->signature !=BIOS_MPCT_SIGNATURE) {
        bv_printf("MPCT structure is not valid.\n");
        return -1;
    }
    bv_printf("\nMPCT Base tabel length: %x", mpct->length_bt);
    bv_printf("\nMPCT rev: %x MPCT Entry Count: %d", mpct->revision, mpct->entry_count);
    bv_printf("\nMPCT Entry Size %x bytes.", mpct->length_ex_table);
    return 1;
}

int bios_parse_mpfp(WORD* mpfp_loc) {
    MPFP* mpfp = (MPFP*)mpfp_loc;
    bv_printf("BIOS probe revealed MPFP table...\n");
    if (mpfp->address == 0x0) {
        bv_printf("MPFP default config not supported yet.\n");
        return -1;
    } else {
        return bios_parse_mpct((WORD*)mpfp->address);
    }
    return -1;
}

// try to find the mp struct in range
int bios_walk_mp(WORD* ebda_probe_loc, WORD size) {
    WORD* c_loc = ebda_probe_loc;
    WORD signature = 0;
    while (c_loc <(ebda_probe_loc+size)) {
        signature = (WORD)*c_loc;
        if (signature == BIOS_MPFP_SIGNATURE) {
            if (!bios_parse_mpfp(c_loc)) {
                return -1;
            }
            return 1;
        } else if (signature == BIOS_MPCT_SIGNATURE) {
            if(!bios_parse_mpct(c_loc)) {
                return -1;
            }
            return 1;
        }
        c_loc+=4;
    }
    return 0;
}
// look for the mpfp info.
int bios_probe_mpfp(WORD ebda_loc) {
   // start where we think the ebda info should be.
    switch (bios_walk_mp((WORD*)BIOS_EBDA_ADDRESS_NORM, BIOS_EBDA_SIZE)) {
        case -1: { // found but could not parse
            return 0;
        }
        break;
        case 1: {
            return 1; // all ok, found and parsed.
        }
        break;
    } // continue looking. BIOS data area now

    switch (bios_walk_mp((WORD*)BIOS_ROM_ADDRESS, BIOS_ROM_SIZE)) {
        case -1: {
            return 0;
        }
        break;
        case 1: {
            return 1;
        }
        break;
    }
    switch (bios_walk_mp((WORD*)ebda_loc,BIOS_EBDA_SIZE)) {
        case 0:
        case -1: {
            return 0;
        }
        break;
        case 1: {
            return 1;
        }
    }

    return 0;
}
// tries to find reported EBDA area, it's not gaurateed to be what we want.
WORD bios_locate_ebda(void) {
    WORD* ebda_ptr = (WORD*) BIOS_EBDA_BASE_POINTER;
    return *ebda_ptr;
}
// probe the bios for info we need to run.
int bios_probe(void) {
    if (!bios_probe_mpfp(bios_locate_ebda())) {
        return 0;
    }
    return 1;
}
