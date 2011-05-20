.global loader           # making entry point visible to linker
.extern kmain            # kmain is defined elsewhere

# setting up the Multiboot header - see GRUB docs for details
.set MODULEALIGN, 1<<0                   # align loaded modules on page boundaries
.set MEMINFO, 1<<1                   # provide memory map
.set FLAGS, MODULEALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC, 0x1BADB002           # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)        # checksum required

# More constants from tutorial
.set CODE_SEL, 0x08	# code segment selector in kernel mode 
.set DATA_SEL, 0x10 # data segment selector in kernel mode
/* .set IDT_ADDR, 0x80000	# IDT start address */
/* .set IDT_SIZE, (256*8)	# IDT has fixed length */
/* .set GDT_ADDR, (IDT_ADDR+IDT_SIZE) */
			# GDT starts after IDT
.set GDT_ENTRIES, 5	# GDT has 5 descriptors
			# null descriptor
			# cs segment descriptor for kernel
			# ds segment descriptor for kernel
			# current process tss
			# current process ldt
.set GDT_SIZE, (8*GDT_ENTRIES)
			# GDT length
/* .set KERNEL_SECT, 72	# Kernel lenght, counted by sectors */
/* .set STACK_BOT, 0xa0000	# stack starts at 640K */

.text
MultiBootHeader:
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

gdt:
    .quad    0x0000000000000000 # null descriptor
    .quad    0x00cf9a000000ffff # cs
    .quad    0x00cf92000000ffff # ds
    .quad    0x0000000000000000 # reserved for further use
    .quad    0x0000000000000000 # reserved for further use   

gdt_48:
    .word .-gdt-1
    .long gdt
# reserve initial kernel stack space
.set STACKSIZE, 0x4000                  # that's 16k.

loader:
    movl $(stack + STACKSIZE), %esp          # set up the stack
    
    # Reset EFLAGS. 
    pushl $0
    popf
    
    push %eax                           # pass Multiboot magic number
    push %ebx                           # pass Multiboot info structure
    
    lgdt gdt_48
    
    movl    $DATA_SEL, %eax
    movw    %ax,    %ds
    movw    %ax,    %es
    movw    %ax,    %fs
    movw    %ax,    %gs
    movw    %ax,    %ss

    call kmain                          # call kernel proper

    cli
hang:
    hlt                                 # halt machine should kernel return
    jmp   hang

.bss

stack:
    .lcomm stack_area, STACKSIZE                     # reserve 16k stack on a doubleword boundary
