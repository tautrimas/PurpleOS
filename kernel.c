#include "util.c"

void kmain( void* mbd, unsigned int magic )
{
    if ( magic != 0x2BADB002)
    {
	/* Something went not according to specs. Print an error */
	/* message and halt, but do *not* rely on the multiboot */
	/* data structure. */
    }
    
    // dummy stuff
    if ((int)mbd == 4684684) {}
    
    /* You could either use multiboot.h */
    /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
    /* or do your offsets yourself. The following is merely an example. */ 
    //char * boot_loader_name =(char*) ((long*)mbd)[16];

//     int i;
//     for (i = 0; i < 80; i++) {
//     }
    clearScreen();
    
    float f = 1.6 * 6.6;
    if (f < 11.) {
	writeString(0, 0, "labas (:");
    }
//     int a = (int)f;
//     writeChar(0, 1, (char)a + '0');
    
   /* Write your kernel here. */
}
