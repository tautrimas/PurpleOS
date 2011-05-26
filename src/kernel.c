#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "task.h"
#include "multiboot.h"
#include "kheap.h"

u32int initial_esp;

u32int max_heap_size = 0x100000;

extern u32int placement_address;

void taskCode() {
    if (getpid() == 2) {
	create_task(&taskCode);
    }
    int i = 0;
    while (1) {
        if (i % 20000000 == 0)
            printf("loop2 %d\n", getpid());
        i++;
    }
}

int factorial(const int n) {
	    if (n == 1) return 1;
	    else return factorial(n - 1) * n;
}

void taskFactorial() {
    int i = 0;
    int ticks = 0;
    while (1) {
        if (i % 1000000 == 0) {
	    ticks++;
	    int x = factorial(ticks % 10 + 1);
	    printf("Factorial pid%d of %d is %d\n", getpid(), ticks - 1, x);
	}
        i++;
    }
}

void kmain(void *mboot_ptr_void, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

//     Find the location of our initial ramdisk.
    multiboot_header_t* mboot_ptr = (multiboot_header_t*)mboot_ptr_void;
    ASSERT(mboot_ptr->mods_count > 0);
    u32int readme_location = *((u32int*)mboot_ptr->mods_addr);
    u32int readme_end = *(u32int*)(mboot_ptr->mods_addr+4);
    printf("README found! %x -- %x\n", readme_location, readme_end);
//     Don't trample our module with placement accesses, please!
    placement_address = readme_end;

//     int readme_l = readme_end - readme_location + 1;
    char* readme_contents = (char*)kmalloc(101);
    memcpy((u8int*)readme_contents, (u8int*)readme_location, 100);
    readme_contents[100] = 0;
    monitor_write(readme_contents);

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();
    init_timer(20);


    // Initialise the initial ramdisk, and set it as the filesystem root.
//    fs_root = initialise_initrd(initrd_location);

    printf("Monitor has been cleared. mboot_ptr: %x\n", (u32int) mboot_ptr);
//     create_task(&taskCode);
//     create_task(&taskFactorial);
//     create_task(&taskFactorial);
    
}
