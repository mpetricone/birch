NASM_MAIN_OPTS="-f elf -o main.o main.asm"
NASM_GDT_OPTS="-f elf -o agdt.o gdt.asm"
NASM_IDT_OPTS="-f elf -o interrupt.o interrupt.asm"
NASM_PORT_OPTS="-f elf -o port.o port.asm"
GCC_OPTS="-Wall -nostdlib -nostartfiles -nodefaultlibs -ffreestanding  -I./inc -c"
LINKER_OPTS="-T birch.lnk -o kernel.elf main.o kmain.o kstring.o kutils.o bvideo.o cgdt.o agdt.o cidt.o interrupt.o panic.o port.o mm_page.o"
nasm $NASM_MAIN_OPTS
nasm $NASM_GDT_OPTS
nasm $NASM_IDT_OPTS
nasm $NASM_PORT_OPTS
i686-elf-gcc $GCC_OPTS -o kmain.o kmain.c
i686-elf-gcc $GCC_OPTS -o bvideo.o bvideo.c
i686-elf-gcc $GCC_OPTS -o kstring.o kstring.c
i686-elf-gcc $GCC_OPTS -o kutils.o kutils.c
i686-elf-gcc $GCC_OPTS -o cgdt.o gdt.c
i686-elf-gcc $GCC_OPTS -o cidt.o idt.c
i686-elf-gcc $GCC_OPTS -o panic.o panic.c
i686-elf-gcc $GCC_OPTS -o mm_page.o mm_page.c
i686-elf-ld $LINKER_OPTS
