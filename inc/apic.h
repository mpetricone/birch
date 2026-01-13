#ifndef _APIC_
#define _APIC_
// might be a bit paranoid with all the volatile here.
// these are LAPIC functions
extern int apic_init(void);
extern volatile WORD apic_read(WORD reg);
extern volatile WORD apic_write(WORD value, WORD reg);
extern volatile WORD apic_owrite(WORD value, WORD reg);
extern volatile WORD apic_enable(void);
extern volatile WORD apic_disable(void);

//these are IOAPIC
extern WORD ioapic_getversion(void);
extern WORD ioapic_getid(void);

// LAPIC internal interrupts
#define APIC_INTERRUPT_TIMER 0x28
#define APIC_INTERRUPT_THERMAL 0x29
#define APIC_INTERRUPT_PERFCNT 0x2A
#define APIC_INTERRUPT_LINT0 0x2B
#define APIC_INTERRUPT_LINT1 0x2C
#define APIC_INTERRUPT_LVTERROR 0x2D


// APIC base default values. had issues moving LAPIC
#define APIC_BASE 0xFEE00000
#define IOAPIC_BASE 0xFEC00000

// LAPIC regs
#define APIC_REG_ID  0x20
#define APIC_REG_VERSION 0x30
#define APIC_REG_SPURIOUS 0xF0
#define APIC_REG_TIMER 0x320
#define APIC_REG_THERMAL 0x330
#define APIC_REG_PERFCNT 0x340
#define APIC_REG_LINT0 0x350
#define APIC_REG_LINT1 0x360
#define APIC_REG_LVTERROR 0x370
#define APIC_INTERRUPT_LVT_CMCI 0x2F0

// IOAPIC regs
#define IOAPIC_REG_ID 0x00
#define IOAPIC_REG_VERSION 0x01
#endif
