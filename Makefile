NASM_OPTS=-f elf
CFLAGS=-Wall -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -I./inc -c
LINKER_OPTS=-T birch.lnk -o kernel.elf
LINKER=i686-elf-ld
CC=i686-elf-gcc
ASM=nasm
KERNEL_OBJECTS=main.o kmain.o kstring.o kutils.o bvideo.o cgdt.o agdt.o cidt.o interrupt.o panic.o port.o mm_page.o  apic.o asm_utils.o cpuchk.o bios_info.o ioapic.o mmlist.o klist.o
KERNEL_HEADERS=kernel.h mb.h bvideo.h kstring.h kutils.h gdt.h idt.h panic.h mm.h types.h apic.h bios.h klist.h
DISK_PATH=/cygdrive/h/boot/kernel.elf
VPATH=src:./inc

all : kernel
birch : kernel

kernel : $(KERNEL_OBJECTS)
	$(LINKER) $(LINKER_OPTS) $(KERNEL_OBJECTS)
main.o : main.asm
	$(ASM) $(NASM_OPTS) main.asm -o main.o

agdt.o : gdt.asm	inc/gdt.h
	$(ASM) $(NASM_OPTS) gdt.asm -o agdt.o

interrupt.o : interrupt.asm  inc/idt.h
	$(ASM) $(NASM_OPTS) interrupt.asm -o interrupt.o

port.o : port.asm
	$(ASM) $(NASM_OPTS) port.asm -o port.o

kmain.o : kmain.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) kmain.c -o kmain.o

bvideo.o : bvideo.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) bvideo.c -o bvideo.o

kstring.o : kstring.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) kstring.c -o kstring.o

kutils.o : kutils.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) kutils.c -o kutils.o

cgdt.o : gdt.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) gdt.c -o cgdt.o

cidt.o : idt.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) idt.c -o cidt.o

panic.o : panic.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) panic.c -o panic.o

mm_page.o : mm_page.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) mm_page.c -o mm_page.o
apic.o : apic.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) apic.c -o apic.o

asm_utils.o : asm_utils.asm $(KERNEL_HEADERS)
	$(ASM) $(NASM_OPTS) asm_utils.asm -o asm_utils.o

chuckeck.o : cpucheck.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) cpucheck.c -o cpucheck.o

bios_info.o : bios_info.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) bios_info.c -o bios_info.o

ioapic.0 : ioapic.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) ioapic.c -o ioapic.o

mmlist.o : mmlist.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) mmlist.c -o mmlist.o
	
klist.o : klist.c $(KERNEL_HEADERS)
	$(CC) $(CFLAGS) klist.c -o klist.o

clean :
	rm *.o
	rm kernel.elf
disk : kernel
	cp kernel.elf $(DISK_PATH)

