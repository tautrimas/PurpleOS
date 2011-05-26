// 
// task.c - Implements the functionality needed to multitask.
//          Written for JamesM's kernel development tutorials.
//

#include "task.h"
#include "paging.h"
#include "common.h"
#include "kheap.h"
#include "monitor.h"

// The currently running task.
volatile task_t *current_task;

// The start of the task linked list.
volatile task_t *ready_queue;

// Some externs are needed to access members in paging.c...
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*,int,int);
extern u32int initial_esp;
extern u32int read_eip();

// The next available process ID.
u32int next_pid = 1;

void initialise_tasking()
{
    // Rather important stuff happening, no interrupts please!
    asm volatile("cli");

    // Relocate the stack so we know where it is.
    move_stack((void*)STACK_TOP, 0x2000);

    // Initialise the first task (kernel task)
    current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;

    // Reenable interrupts.
    asm volatile("sti");
}

void move_stack(void *new_stack_start, u32int size)
{
  u32int i;
  // Allocate some space for the new stack.
  for( i = (u32int)new_stack_start;
       i >= ((u32int)new_stack_start-size);
       i -= 0x1000)
  {
    // General-purpose stack is in user-mode.
    alloc_frame( get_page(i, 1, current_directory), 0 /* User mode */, 1 /* Is writable */ );
  }
  
  // Flush the TLB by reading and writing the page directory address again.
  u32int pd_addr;
  asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
  asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

  // Old ESP and EBP, read from registers.
  u32int old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
  u32int old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

  // Offset to add to old stack addresses to get a new stack address.
  u32int offset            = (u32int)new_stack_start - initial_esp;

  // New ESP and EBP.
  u32int new_stack_pointer = old_stack_pointer + offset;
  u32int new_base_pointer  = old_base_pointer  + offset;

  // Copy the stack.
  memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

  // Backtrace through the original stack, copying new values into
  // the new stack.  
  for(i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4)
  {
    u32int tmp = * (u32int*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer
    // and remap it. This will unfortunately remap ANY value in this range, whether they are
    // base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp))
    {
      tmp = tmp + offset;
      u32int *tmp2 = (u32int*)i;
      *tmp2 = tmp;
    }
  }

  // Change stacks.
  asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
  asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

// In timer.c
extern int timer_lock_free;

inline void pause_tasking() {
    timer_lock_free = 0;
}

inline void continue_tasking() {
    timer_lock_free = 1;
}

u32int new_directory_address;

u32int switch_task(u32int oldEsp) {
    // If we haven't initialised tasking yet, just return.
    if (!timer_lock_free) {
	new_directory_address = current_task->page_directory->physicalAddr;
	return oldEsp;
    }
    
    pause_tasking();
    
//     printf("switch %d*", current_task->id);
    
    if (!current_task) {
        return oldEsp;
    }
    current_task->esp = oldEsp;

    //Now switch what task we're on
    current_task = current_task->next;
    // If we fell off the end of the linked list start again at the beginning.
    if (!current_task) {
        current_task = ready_queue;
    }
    
    current_directory = current_task->page_directory;
    new_directory_address = current_task->page_directory->physicalAddr;
    
    continue_tasking();

    return current_task->esp; //Return new stack pointer to ASM
}

int create_task(void (*thread)()) {
    
    asm volatile("cli");
    
//     u32int *stack;

//     task_t *parent_task = (task_t*)current_task;

    // Clone the address space.
    page_directory_t *directory = clone_directory(current_directory);

    u32int current_esp;
    u32int current_ebp;
    asm volatile ("mov %%esp, %0" : "=r"(current_esp));
    asm volatile ("mov %%ebp, %0" : "=r"(current_ebp));

    asm volatile("mov %%cr3, %%ebx; \
    mov %0, %%cr3; \
    mov %%ebp, %%esp; \
	push $0x0202; \
	push $0x08; \
	push %2; \
	push $0; \
	push $32; \
	push $0; \
	push $0; \
	push $0; \
	push $0; \
	push $0; \
	push $0; \
	push $0; \
	push $0; \
	push $0x10; \
    mov %%ebx, %%cr3; \
    mov %1, %%esp; \
    " : : "r"(directory->physicalAddr), "r"(current_esp), "r"((u32int)thread) : "ebx");
    
    int stack_depth = 14 * 4;
    
    // Create a new process.
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

    new_task->esp = current_ebp - stack_depth;
    new_task->id = next_pid++;
    new_task->page_directory = directory;
    new_task->next = 0;
    
    task_t *tmp_task = (task_t*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    tmp_task->next = new_task;

//     stack = (u32int*)STACK_TOP; //This makes a pointer to the stack for us
// 
//     //First, this stuff is pushed by the processor
//     *--stack = 0x0202; //This is EFLAGS
//     *--stack = 0x08;   //This is CS, our code segment
//     *--stack = (u32int)thread; //This is EIP
// 
//     // error code and irq number
//     *--stack = 0;
//     *--stack = 32;
// 
//     //Next, the stuff pushed by 'pusha'
//     *--stack = 0; //EDI
//     *--stack = 0; //ESI
//     *--stack = 0; //EBP
//     *--stack = 0; //Just an offset, no value
//     *--stack = 0; //EBX
//     *--stack = 0; //EDX
//     *--stack = 0; //ECX
//     *--stack = 0; //EAX
// 
//     *--stack = 0x10; // ds

    //Now these are the data segments pushed by the IRQ handler
//     *--stack = 0x10; //DS
//     *--stack = 0x10; //ES
//     *--stack = 0x10; //FS
//     *--stack = 0x10; //GS

//     new_task->esp = (u32int)stack; //Update the stack pointer

    asm volatile("sti");

    return new_task->id;
}

int getpid()
{
    return current_task->id;
}
