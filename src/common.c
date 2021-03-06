// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.

#include "common.h"
#include "monitor.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

int max(int a, int b) {
    if (a > b) return a;
    else return b;
}

int min(int a, int b) {
    if (a < b) return a;
    else return b;
}

int strlen(const char* str) {
    int i;
    for (i = 0; str[i] != 0; i++) {}
    return i;
}

void* memset(void *dest, int val, unsigned int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
    return dest;
}

void* memcpy(u8int *dest, const u8int *src, u32int len)
{
     const u8int *sp = (const u8int *)src;
     u8int *dp = (u8int *)dest;
     for(; len != 0; len--) *dp++ = *sp++;
     return dest;
}

extern void panic(const char *message, const char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    monitor_write("PANIC(");
    monitor_write(message);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");
    // Halt by going into an infinite loop.
    asm volatile ("hlt");
    for(;;);
}

extern void panic_assert(const char *file, u32int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    monitor_write("ASSERTION-FAILED(");
    monitor_write(desc);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");
    // Halt by going into an infinite loop.
    asm volatile ("hlt");
    for(;;);
}
