#ifndef _BVIDEO_
#define _BVIDEO_
#include "mb.h"
extern volatile  BYTE bv_color;
extern volatile BYTE bv_tcolor;
extern void bv_putc(char c);
extern void bv_puts(char* str);
extern void bv_cls(void);
extern void bv_setcolor(BYTE color);
extern void bv_printf(char* out,...);
extern void bv_init(void);
extern void bcurs_setpos(void);
extern void bv_indent_to(int ind);

typedef struct _vga_ctrl_regs {
    WORD address_reg;
    WORD data_reg;
} vga_ctrl_reg;
// vga ports and whatnot
#define VGA_MISC_OUTREG 0x3C2
#define VGA_MISC_INREG  0x3CC
#define VGA_ADDRESS_REG_HIGH 0x3D4
#define VGA_ADDRESS_REG_LOW 0x3B4
#define VGA_DATA_REG_HIGH 0x3D5
#define VGA_DATA_REG_LOW 0x3B5
// Color helper macros
#define BVC_PUTS(s, c) bv_tcolor = bv_color;bv_color=c;bv_puts(s);bv_color=bv_tcolor;

#endif
