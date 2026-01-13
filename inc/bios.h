#ifndef _BIOS_
#define _BIOS_
#include "types.h"
// Bios function prototypes & defs.
// expect to find EBDA here.
#define BIOS_EBDA_ADDRESS_NORM 0x0009FC00
#define BIOS_EBDA_SIZE 0x00000400
#define BIOS_ROM_ADDRESS 0x000F0000
#define BIOS_ROM_SIZE 0x0000FFFF

#define BIOS_EBDA_BASE_POINTER 0x0000040E

// _MP_
#define BIOS_MPFP_SIGNATURE 0x05504D05
#define BIOS_MPCT_SIGNATURE 0x504D4350


typedef struct MPFP_s {
    WORD signature;
    WORD address;
    BYTE length;
    BYTE spec_rev;
    BYTE checksum;
    BYTE feature[5];

} __attribute__((packed)) MPFP;

typedef struct MPCT_s {
    WORD signature;
    USHORT length_bt;
    UCHAR revision;
    UCHAR checksum;
    DWORD id_oem;
    WORD id_product[3];
    WORD table_oem;
    USHORT length_oem_table;
    USHORT entry_count;
    WORD loc_apic;
    USHORT length_ex_table;
    UCHAR checksum_ex_table;
} __attribute__((packed)) MPCT;

/*
   Many of the structures below are based off of intel Mulitproc docs.
   For tables, static values and so on refer to intel doc 24201606.
*/
// Multiproc Proessor table.
typedef struct MPETProc_s {
    BYTE entry_type; // should be 0
    BYTE lapic_id;
    BYTE lapic_version;
    BYTE cpu_flags; // bit 0=1 = proc unusable. bit 1 = 1 = bootstrap proc
    WORD cpu_signature; // steping and so on. 1111 1111 1111 = not intel CPU. table 4-5 of intel multiproc doc.
    WORD feature_flags; // misc flags we probably don't care about at this point. table 4-6 of multiproc doc.
    DWORD reserved; // technically 2 32 bits acording to the table. but who cares?
} __attribute__((packed)) MPETProc; // decided to pack incase I map directly to memory. Can be unpacked if this is not the case.

// Bus entries
typedef struct MPETBus_s {
    BYTE entry_type; // 1
    BYTE bus_id;
    USHORT reserved;
    WORD bus_type; //table 4-8.I should probably make some macros for these, there are many.
} __attribute__((packed)) MPETBus;

// local apic entries, contains memory address space
typedef struct MPETIOApic_s {
    BYTE entry_type; // 2
    BYTE apic_id;
    BYTE apic_version;
    BYTE apic_flags; // b0=1 = unusable
    WORD apic_location; // this, you may want.
} __attribute__((packed)) MPETIOApic;

// local apic assignment entries - generic for IO & Local.
typedef struct MPETApicAssignment {
    BYTE entry_type; // 3
    BYTE interrupt_type; //table 4-11
    USHORT int_flag; //table 4-10.. polarity?/triggermode? -- crap I sense more learning.
    BYTE source_bus_id;
    BYTE source_bus_irq;
    BYTE dest_apic_id; //0FFh=all
    BYTE dest_apic_intin;
} __attribute__((packed)) MPETIOApicAssignment;
// values for the entry types seen above
#define MPET_TYPE_PROC 0
#define MPET_TYPE_BUS 1
// IO APIC Entries
#define MPET_TYPE_IOAPIC 2
// IO APIC Assignment
#define MPET_TYPE_IOA_ASS 3
// LAPIC Assignment
#define MPET_TYPE_LA_ASS 4

extern int bios_probe(void);
#endif
