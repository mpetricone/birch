
#define VIDEO_MEM_START 0xb8000
#define VIDEO_MEM_UPPER 0xB8FA0
#include "types.h"
#include "kstring.h"
#include "kutils.h"
#include "bvideo.h"
int bv_pos = 0;
volatile BYTE bv_color = 0x07, bv_tcolor;
volatile BYTE *bv_memory = (BYTE*)VIDEO_MEM_START;
vga_ctrl_reg vga_regs;
// seems silly but these externs help putc
extern void bv_puts(char* str);
void bv_indent_to(int ind) {
    if (ind > 80) // newline
        return;
    bv_pos = ((bv_pos/160)*160)+ind*2;
    bcurs_setpos();
}

void bv_putc(char c) {
    if ((bv_memory+bv_pos) >= (BYTE*)(VIDEO_MEM_UPPER)) {
        bv_pos = 0;
     //   bv_cls();
    }
    switch (c) {
        case '\n': {
        bv_pos = ((bv_pos/160)+1)*160;
        return;
        }
        case '\t': {
            bv_puts(STRING_TAB);
            return;
        }
    }
    bv_memory[bv_pos++] = c;
    bv_memory[bv_pos++] = bv_color;
    bcurs_setpos();
}

void bv_puts(char* str) {
    while (*str!=0) {
        bv_putc(*str++);
    }
}

void bv_cls(void) {
    int i;
    bv_pos = 0;
    for (i = 0; i<=(80*25); i++) {
        bv_putc(' ');
    }
    bv_pos = 0;
}
// Very simple.
void bv_printf(char* out,...) {
    char buff[24];
    __builtin_va_list argv;
    __builtin_va_start(argv, out);
    while (*out != 0) {
        if (*out != '%') {
            bv_putc(*out++);
        } else {
            out++;
            switch(*out++) {
                case 'd': {
                    bv_puts(kitoa(__builtin_va_arg(argv,int),10, buff, 24));
                }
                break;
                case 'x': {
                    bv_puts(PREFIX_HEX);
                    bv_puts(kitoa(__builtin_va_arg(argv,unsigned int), 16, buff, 24));
                }
                break;
                case 'y': {
                    bv_puts(kitoa(__builtin_va_arg(argv,unsigned int), 16, buff, 24));
                }
                break;
                case 'b': {
                    bv_puts(kitoa(__builtin_va_arg(argv,unsigned int), 2, buff, 24));
                }
                break;
                case 'z': {
                    bv_puts(kitoa_zeros(__builtin_va_arg(argv,unsigned int), buff, 24));
                }
                break;
                case 's': {
                    bv_puts(__builtin_va_arg(argv, char*));
                }
                break;
                case 'c': {
                    bv_putc((char)__builtin_va_arg(argv, int));
                }
                break;
            }
        }
    }
    __builtin_va_end(argv);

}

void bv_setcolor(BYTE color) {
    bv_color = color;
}

// sets the vga ports info
void bv_init(void) {
    // this will get the propper for for the VGA vontrol regs
    BYTE flags = inportb(VGA_MISC_INREG);
    if (flags &0x1) {
        vga_regs.address_reg =VGA_ADDRESS_REG_HIGH;
        vga_regs.data_reg = VGA_DATA_REG_HIGH;
    } else {
        vga_regs.address_reg = VGA_ADDRESS_REG_LOW;
        vga_regs.data_reg = VGA_DATA_REG_LOW;
    }
    bcurs_setpos();
}
// cursor functions

void bcurs_setpos(void) {
    int t_pos = bv_pos/2;
    outportb(vga_regs.address_reg, 0x0E);
    outportb(vga_regs.data_reg,(BYTE) ((t_pos>>8) &0xFFFF) );
    outportb(vga_regs.address_reg, 0xF);
    outportb(vga_regs.data_reg,(BYTE)(t_pos &0xFF));
}
