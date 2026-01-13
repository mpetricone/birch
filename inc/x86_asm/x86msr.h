#ifndef _X86MSR_
#define _X86MSR_
// This may have been taken from an old  LINUX kernel, I do not remember, was looking for how to use MSRs and ran into extactly the macros/asm that was needed.
#define rdmsr(msr,low,high)  asm volatile ("rdmsr" :"=d"(high), "=a" (low) : "c"(msr))
#define wrmsr(msr, low, high) asm volatile ("wrmsr" : :"a"(low), "d"(high), "c"(msr))

#define x8664MSR_EFER 0xC0000080
#define MSR_IA32_APIC_BASE 0x1b
#endif
