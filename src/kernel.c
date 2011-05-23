#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"

void kmain(void *mboot_ptr)
{
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();  
    
    monitor_clear();  
    printf("Monitor has been cleared. mboot_ptr: %x\n", (u32int) mboot_ptr);
    
    initialise_paging();
    
    printf("Hello, paging world!\n");
    
    /* You could either use multiboot.h */
    /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
    /* or do your offsets yourself. The following is merely an example. */ 
    //char * boot_loader_name =(char*) ((long*)mbd)[16];

    
//     asm volatile ("jmp dummyFunctionL" : : : "memory", "cc");
    
    /*int addr;
//     asm("call magic; magic: pop %%eax; movl %%eax, %0;" : "=r"(addr) : : "eax");
    asm("movl %%ss, %0;" : "=r"(addr) : : );
    printf("Register ss: %x\n", addr);*/
    
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    
//     asm volatile("sti");
//     init_timer(50);
    
    u32int *ptr = (u32int*)0x150003;
    u32int do_page_fault = *ptr;
    printf("got %d\n", do_page_fault);
}
