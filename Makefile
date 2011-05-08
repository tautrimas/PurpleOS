all: floppy.img

kernel.bin: kernel.o linker.ld loader.o
	ld -T linker.ld -o $@ loader.o kernel.o -melf_i386

kernel.o: kernel.c
	gcc -o kernel.o -c kernel.c -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32

loader.o: loader.s
	nasm -f elf -o loader.o loader.s

floppy.img: stage1 stage2 pad pad2 kernel.bin
	cat stage1 stage2 pad kernel.bin pad2 > floppy.img
	
pad:
	dd if=/dev/zero of=pad bs=1 count=750

pad2:
	dd if=/dev/zero of=pad2 bs=1 count=1367200

clean:
	rm -f pad2 pad kernel.bin kernel.o loader.o floppy.img

run: floppy.img
	VBoxManage startvm "os test"
