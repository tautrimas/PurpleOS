.PHONY: clean all run

all: floppy.img

srcs=src/kernel.c src/monitor.c src/common.c
objs=$(srcs:.c=.o)

CC=gcc
AS=as
LD=ld

kernel.bin: linker.ld src/loader.o $(objs)
	$(LD) -T linker.ld -o $@ src/loader.o $(objs) -melf_i386

%.o: %.c
	$(CC) -o $@ -c $^ -Wall -Wextra -nostdlib \
	-nostartfiles -nodefaultlibs -m32

src/loader.o: src/loader.s
	$(AS) --32 -o $@ $^
	
floppy.img: kernel.bin
	sudo sh create_floppy.sh

clean:
	rm -f kernel.bin $(objs) src/loader.o floppy.img

run: floppy.img
	VBoxManage startvm "os test";
