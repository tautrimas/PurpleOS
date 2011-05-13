.global loader           # making entry point visible to linker
.extern kmain            # kmain is defined elsewhere
 
# setting up the Multiboot header - see GRUB docs for details
.set MODULEALIGN, 1<<0                   # align loaded modules on page boundaries
.set MEMINFO, 1<<1                   # provide memory map
.set FLAGS, MODULEALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC, 0x1BADB002           # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)        # checksum required
 
.text
MultiBootHeader:
   .long MAGIC
   .long FLAGS
   .long CHECKSUM
 
# reserve initial kernel stack space
.set STACKSIZE, 0x4000                  # that's 16k.
 
loader:
   movl $(stack + STACKSIZE), %esp          # set up the stack
   push %eax                           # pass Multiboot magic number
   push %ebx                           # pass Multiboot info structure
 
   call  kmain                       # call kernel proper
 
   cli
hang:
   hlt                                # halt machine should kernel return
   jmp   hang
 
.bss
stack:
   .lcomm stack_area, STACKSIZE                     # reserve 16k stack on a doubleword boundary
