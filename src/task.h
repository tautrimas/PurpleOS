// 
// task.h - Defines the structures and prototypes needed to multitask.
//          Written for JamesM's kernel development tutorials.
//

#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "paging.h"

// This structure defines a 'task' - a process.
typedef struct task
{
    int id;                // Process ID.
    u32int esp;       // Stack and base pointers.
    page_directory_t *page_directory; // Page directory.
    struct task *next;     // The next task in a linked list.
} task_t;

// Initialises the tasking system.
void initialise_tasking();

// Called by the timer irq asm code. Asm passes current esp, expects new esp in eax.
u32int switch_task(u32int oldEsp);

// Forks the current process, spawning a new one with a different
// memory space.
//int fork();

// creates task at function thread. Returns pid
int create_task(void (*thread)());

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack(void *new_stack_start, u32int size);

// Returns the pid of the current process.
int getpid();

#endif
