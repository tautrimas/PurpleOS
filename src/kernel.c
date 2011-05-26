#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "task.h"

u32int initial_esp;

u32int max_heap_size = 0x100000;

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

void kmain(void *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    // Find the location of our initial ramdisk.
//    ASSERT(mboot_ptr->mods_count > 0);
//    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
//    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
//    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();
    init_timer(20);


    // Initialise the initial ramdisk, and set it as the filesystem root.
//    fs_root = initialise_initrd(initrd_location);

    printf("Monitor has been cleared. mboot_ptr: %x\n", (u32int) mboot_ptr);
//     create_task(&taskCode);
    create_task(&taskFactorial);
    create_task(&taskFactorial);
//     int i = 0;
//     while (1) {
//         if (i % 10000000 == 0)
//             printf("loop %d\n", getpid());
//         i++;
//     }
    // Create a new process in a new address space which is a clone of this.
//     int ret = fork();
// //     int ret = 0;
//     if (ret != 0) {
//         while (1) {printf("parent\n");}
//     }
//     else {
//         while (1) {printf("child\n");}
//     }
//    monitor_write_hex(ret);
//    monitor_write(", and getpid() returned ");
//    monitor_write_hex(getpid());
//    monitor_write("\n============================================================================\n");
}
