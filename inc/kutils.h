#ifndef _KUTILS_
#define _KUTILS_
#define PREFIX_HEX "0x"
#define STRING_TAB "     "

#define BOOT_CONSOLE_AOK bv_indent_to(40); BVC_PUTS("\t<AOK>\n", 0xF);
#define BOOT_CONSOLE_AOK_EXT(s) bv_indent_to(40); BVC_PUTS("\t[AOK->",0xF); BVC_PUTS(s, 0xF); BVC_PUTS("]\n", 0xF);
#define BOOT_CONSOLE_PASSABLE bv_indent_to(40); BVC_PUTS("\t<PASSABLE>\n", 0xE);
#define BOOT_CONSOLE_PASSALBE_EXT(s) bv_indent_to(40); BVC_PUTS("\t[PASSABLE->",0xE); BVC_PUTS(s, 0xE); BVC_PUTS("]\n", 0xF);
#define BOOT_CONSOLE_FAIL bv_indent_to(40); BVC_PUTS("\t<FAIL>\n",0x4);
extern void memcpy(void* dest, const void* src,UINT size);
void memset(void* data, BYTE info,UINT size);
// inoutports
extern BYTE inportb(WORD port);
extern void outportb(WORD port, BYTE b);
extern WORD get_word(WORD address);
extern WORD set_word(WORD address, WORD value);
extern WORD invalidate_tlb(WORD address);
extern int cpu_check(void);
#endif
