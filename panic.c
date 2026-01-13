#include "inc/panic.h"
#include "inc/bvideo.h"
// PANIC!!! new and improved. No really it's improved.
void panic_boot(char* fail_text) {
    asm("cli");
    bv_puts(fail_text);
    for (;;) {asm("hlt");}
}
// if this happens, something went very wrong, or someone likes this screen alot.
void panic(WORD panic_code,WORD info, WORD func, isr_info* interrupt) {
    bv_color = 0x49;
    bv_cls();
    bv_color = 0x0A;
    bv_puts(" _______  _______  _       _________ _______  _                                 ");
    bv_puts("(  ____ )(  ___  )( (    /|\\__   __/(  ____ \\( )                                ");
    bv_puts("| (    )|| (   ) ||  \\  ( |   ) (   | (    \\/| |                                ");
    bv_puts("| (____)|| (___) ||   \\ | |   | |   | |      | |                                ");
    bv_puts("|  _____)|  ___  || (\\ \\) |   | |   | |      | |                                ");
    bv_puts("| (      | (   ) || | \\   |   | |   | |      (_)                                ");
    bv_puts("| )      | )   ( || )  \\  |___) (___| (____/\\ _                                 ");
    bv_puts("|/       |/     \\||/    )_)\\_______/(_______/(_)                                ");
    bv_color = 0x49;
    bv_puts("--------------------------------------------------------------------------------");
    bv_printf("KERNEL PANIC! PANIC CODE:%d, INFO: %x, FUNCTION ADDRESS:%x\n", panic_code, info, func);
    if (interrupt) {
        bv_printf("Interrupt caused panic. State info follows.\n");
        bv_puts("--------------------------------------------------------------------------------");
        bv_printf("eax:%z\tebx:%z\necx:%z\tedx:%z\n", interrupt->eax, interrupt->ebx, interrupt->ecx, interrupt->edx);
        bv_printf("edi:%z\tesi:%z\nedp:%z\tesp:%z\n", interrupt->edi, interrupt->esi, interrupt->edp, interrupt->esp);
        bv_puts("--------------------------------------------------------------------------------");
        bv_printf("cs:%z   ss:%z\n", interrupt->cs, interrupt->ss);
        bv_printf("gs:%z   fs:%z   es%z   gs%z\n", interrupt->gs, interrupt->fs, interrupt->es, interrupt->gs);
        bv_puts("--------------------------------------------------------------------------------");
        bv_printf("eip:%z\neflags:%z\nuseresp:%z\n", interrupt->eip, interrupt->eflags, interrupt->useresp);
        bv_puts("--------------------------------------------------------------------------------");
        bv_color = 0x0A;
        bv_puts("                                                                                ");
    } else {
        bv_puts("--------------------------------------------------------------------------------");
        bv_color=0x0A;
        bv_printf(" _______             _        _______  _                                        ");
        bv_printf("(  ___  )|\\     /|  ( (    /|(  ___  )( )                                       ");
        bv_printf("| (   ) || )   ( |  |  \\  ( || (   ) || |                                       ");
        bv_printf("| |   | || (___) |  |   \\ | || |   | || |                                       ");
        bv_printf("| |   | ||  ___  |  | (\\ \\) || |   | || |                                       ");
        bv_printf("| |   | || (   ) |  | | \\   || |   | |(_)                                       ");
        bv_printf("| (___) || )   ( |  | )  \\  || (___) | _                                        ");
        bv_printf("(_______)|/     \\|  |/    )_)(_______)(_)                                       ");
        bv_color = 0x49;
        bv_printf("There was no other info available.\n");
    }

    for (;;) {asm("hlt");}
}
