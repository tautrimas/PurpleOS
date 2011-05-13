#include "util.c"

void kmain( void* mbd, unsigned int magic )
{
    if ( magic != 0x2BADB002)
    {
	/* Something went not according to specs. Print an error */
	/* message and halt, but do *not* rely on the multiboot */
	/* data structure. */
	return;
    }
    
    // dummy stuff
    if ((int)mbd == 4684684) {}
    
    /* You could either use multiboot.h */
    /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
    /* or do your offsets yourself. The following is merely an example. */ 
    //char * boot_loader_name =(char*) ((long*)mbd)[16];

    clearScreen();
    
//     asm volatile ("jmp dummyFunctionL" : : : "memory", "cc");
    
    int addr;
//     asm("call magic; magic: pop %%eax; movl %%eax, %0;" : "=r"(addr) : : "eax");
    asm("movl %%ss, %0;" : "=r"(addr) : : );
    printf("Register: %x\n", addr);
}
