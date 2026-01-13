// Multboot Header Info
#ifndef _MB_
#define _MB_
#include "types.h"

#define MB_MAGIC 0x1BADB002
typedef struct _multboot_mem_info {
    WORD mem_lower;
    WORD mem_upper;
} mb_memory;

typedef struct _multiboot_mod_info {
    WORD count;
    WORD address;
} mb_mod;

typedef struct _multiboot_mem_map {
    WORD length;
    WORD address;
} mb_mm;

typedef struct _multiboot_mem_descriptor {
    WORD size;
    DWORD base_address;
    DWORD length;
    WORD type;
} mb_mem_descriptor;

typedef struct _multiboot_drives {
    WORD length;
    WORD address;
} mb_drives;

typedef struct _multiboot_drive_descriptor {
    WORD size;
    WORD number;
    BYTE mode;
    USHORT cylinders;
    BYTE heads;
    BYTE sectors;
    USHORT ports[0];
} mb_drive_descriptor;

typedef struct _multiboot_video_info {
    WORD controler_info;
    WORD mode_info;
    USHORT mode;
    USHORT interface_seg;
    USHORT interface_off;
    USHORT interface_len;
} mb_video;

typedef struct _multiboot_aout_info {
    WORD tabsize;
    WORD strsize;
    WORD address;
    WORD reserved;
} mb_aout_info;

typedef struct _multiboot_elf_info {
    WORD num;
    WORD size;
    WORD address;
    WORD shndx;
}mb_elf_info;

typedef struct _multiboot_mods_descriptor {
    WORD start;
    DWORD end;
    WORD string;
    WORD reserved;
} mb_mod_descriptor;

typedef struct _multiboot_apm_descriptor {
    USHORT version;
    USHORT cseg;
    WORD offest;
    USHORT cseg_16;
    USHORT dseg;
    USHORT flags;
    USHORT cseg_len;
    USHORT cseg_l6_len;
    USHORT dseg_len;
} mb_apm_descriptor;

typedef struct _multiboot_info {
    WORD flags;
    mb_memory memory;
    WORD boot_device;
    WORD command_line;
    mb_mod mods;
    union {
        mb_aout_info aout_info;
        mb_elf_info elf_info;
    } syms;
    mb_mm memory_map;
    mb_drives drives;
    WORD config_table;
    WORD boot_loader_name;
    WORD apm_name;
    mb_video vid_info;
} __attribute__((packed)) mb_info;


#endif
