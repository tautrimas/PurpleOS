.PHONY: clean all run

all: floppy.img

kernel.bin: src/kernel.o linker.ld src/loader.o
	ld -T linker.ld -o $@ src/loader.o src/kernel.o -melf_i386

src/kernel.o: src/kernel.c src/util.c
	gcc -o $@ -c src/kernel.c -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32

src/loader.o: src/loader.s
	as --32 -o $@ $^
	
floppy.img: kernel.bin
	sudo sh create_floppy.sh

clean:
	rm -f kernel.bin src/loader.skernel.o src/loader.sloader.o floppy.img

run: floppy.img
	VBoxManage startvm "os test";
