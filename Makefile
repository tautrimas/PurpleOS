.PHONY: clean all run

all: floppy.img

srcs=src/kernel.c src/monitor.c src/common.c src/descriptor_tables.c src/isr.c
objs=$(srcs:.c=.o)
srcs_nasm=src/loader.s src/gdt.s src/interrupt.s
objs_nasm=$(srcs:.s=.o)
srcs_as=src/loader.asm
objs_as=$(srcs:.asm=.o)

CC=gcc
AS=as
NASM=nasm
LD=ld

kernel.bin: linker.ld src/loader.o $(objs)
	$(LD) -T linker.ld -o $@ src/loader.o $(objs) -melf_i386

%.o: %.c
	$(CC) -o $@ -c $^ -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32

%.o: %.s
	$(NASM) -f elf -o $@ $^

%.o: %.asm
	$(AS) --32 -o $@ $^
	
floppy.img: kernel.bin
	sudo sh create_floppy.sh

clean:
	rm -f kernel.bin $(objs) $(objs_nasm) $(objs_as) floppy.img

run: floppy.img
	VBoxManage startvm "os test";
