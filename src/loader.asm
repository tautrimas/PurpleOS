.global mboot
.extern code
.extern bss
.extern end

.set MBOOT_PAGE_ALIGN, 1<<0                   # align loaded modules on page boundaries
.set MBOOT_MEM_INFO, 1<<1                   # provide memory map
# NOTE: We do not use MBOOT_AOUT_KLUDGE. It means that GRUB does not
# pass us a symbol table.
.set MBOOT_HEADER_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO  # this is the Multiboot 'flag' field
.set MBOOT_HEADER_MAGIC, 0x1BADB002           # 'magic number' lets bootloader find the header
.set MBOOT_CHECKSUM, -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)        # checksum required

# More constants from tutorial

.balign 4

mboot:
    .long MBOOT_HEADER_MAGIC
    .long MBOOT_HEADER_FLAGS
    .long MBOOT_CHECKSUM

    .long mboot
    .long code
    .long bss
    .long end
    .long start
    
.global start
.extern kmain            # kmain is defined elsewhere

# reserve initial kernel stack space
.set STACKSIZE, 0x4000                  # that's 16k.

start:
#    movl $(stack + STACKSIZE), %esp          # set up the stack
    
    # Reset EFLAGS. 
#    pushl $0
#    popf

    push %esp
    push %ebx                           # pass Multiboot info structure

    cli

    call kmain                          # call kernel proper
    
hang:
    hlt                                 # halt machine should kernel return
    jmp   hang

#.bss

#stack:
#    .lcomm stack_area, STACKSIZE                     # reserve 16k stack on a doubleword boundary
