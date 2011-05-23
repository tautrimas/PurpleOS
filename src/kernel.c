#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "task.h"

u32int initial_esp;

void kmain(void *mboot_ptr, u32int initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();
    // Initialise the PIT to 100Hz
//    asm volatile("sti");
//    init_timer(50);

    // Find the location of our initial ramdisk.
//    ASSERT(mboot_ptr->mods_count > 0);
//    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
//    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
//    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
//    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
//    fs_root = initialise_initrd(initrd_location);

    printf("Monitor has been cleared. mboot_ptr: %x\n", (u32int) mboot_ptr);
    // Create a new process in a new address space which is a clone of this.
//    int ret = fork();
    int ret = 0;
    if (ret != 0) {
        printf("parent");
    }
    else {
        printf("child");
    }
//    monitor_write_hex(ret);
//    monitor_write(", and getpid() returned ");
//    monitor_write_hex(getpid());
//    monitor_write("\n============================================================================\n");
}
