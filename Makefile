all: floppy.img

kernel.bin: kernel.o linker.ld loader.o
	ld -T linker.ld -o $@ loader.o kernel.o -melf_i386

kernel.o: kernel.c
	gcc -o kernel.o -c kernel.c -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32 -Werror

loader.o: loader.s
	nasm -f elf -o loader.o loader.s

floppy.img: stage1 stage2 pad pad2 kernel.bin
	cat stage1 stage2 pad kernel.bin pad2 > floppy.img
	
pad:
	dd if=/dev/zero of=pad bs=1 count=750

pad2: kernel.bin pad stage1 stage2
	size=`cat kernel.bin pad stage1 stage2 | wc --bytes`; \
	size=$$((1474560-$$size)); \
	dd if=/dev/zero of=pad2 bs=1 count=$$size;

clean:
	rm -f pad2 pad kernel.bin kernel.o loader.o floppy.img

run: floppy.img
	VBoxManage startvm "os test"
