.PHONY: clean all run

all: floppy.img

kernel.bin: kernel.o linker.ld loader.o
	ld -T linker.ld -o $@ loader.o kernel.o -melf_i386

kernel.o: kernel.c util.c
	gcc -o $@ -c kernel.c -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32

loader.o: loader.s
	as --32 -o $@ loader.s
	
floppy.img: kernel.bin
	sudo sh create_floppy.sh

clean:
	rm -f kernel.bin kernel.o loader.o floppy.img

run: floppy.img
	VBoxManage startvm "os test";
