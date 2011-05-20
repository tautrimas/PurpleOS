#include "monitor.h"

void kmain(void* mbd)
{
    // dummy stuff
    if ((int)mbd == 4684684) {}
    
    /* You could either use multiboot.h */
    /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
    /* or do your offsets yourself. The following is merely an example. */ 
    //char * boot_loader_name =(char*) ((long*)mbd)[16];

    monitor_clear();
    
//     asm volatile ("jmp dummyFunctionL" : : : "memory", "cc");
    
    int addr;
//     asm("call magic; magic: pop %%eax; movl %%eax, %0;" : "=r"(addr) : : "eax");
    asm("movl %%ss, %0;" : "=r"(addr) : : );
    printf("Register ss: %x\n", addr);
    
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
}
