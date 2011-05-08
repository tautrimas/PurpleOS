.PHONY: clean all run

all: kernel.bin

kernel.bin: kernel.o linker.ld loader.o
	ld -T linker.ld -o $@ loader.o kernel.o -melf_i386

kernel.o: kernel.c
	gcc -o kernel.o -c kernel.c -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32 -Werror

loader.o: loader.s
	nasm -f elf -o loader.o loader.s

clean:
	rm -f kernel.bin kernel.o loader.o floppy.img

run:
	VBoxManage startvm "os test";
