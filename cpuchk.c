// Boot time check of CPU properties
#include "inc/types.h"
#include "inc/bvideo.h"
#define CPUCHECK_PAE 0x40
#define CPUCHECK_PAT 0x10000
#define CPUCHECK_APIC 0x200
#define CPUCHECL_X2APIC 0x200000
/* Check the processor attributes
 * returns 0 on fail, 1 on pass, 2 on pass with an x2APIC reported
*/
int cpu_check(void) {
    WORD d,c, fail = 0, x2APIC = 0;
    asm  volatile ("cpuid ": "=c"(c),"=d"(d): "a"(1));
    if ((c &CPUCHECL_X2APIC)) {
        x2APIC = 1;
    }
    if (!(d&CPUCHECK_APIC)) {
        fail++;
    }
    if (!(d&CPUCHECK_PAE)) {
        fail++;
    }
    if (!(d&CPUCHECK_PAT)) {
        fail++;
    }
    if (fail) {
        return 0;
    }
    if (x2APIC) {
        return 2;
    }
    return 1;
}
