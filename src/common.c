// common.c -- Defines some global functions.
//             From JamesM's kernel development tutorials.

#include "common.h"

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

// Copy len bytes from src to dest.
extern inline void* memcpy(u8int *dest, const u8int *src, u32int n)
{
    __asm__("cld\n\t"
        "rep\n\t"
        "movsb"
        ::"c" (n),"S" (src),"D" (dest)
        :"cx","si","di");
    return dest;
}

// Write len copies of val into dest.
extern inline void* memset(void * s,char c,int count)
{
    __asm__("cld\n\t"
        "rep\n\t"
        "stosb"
        ::"a" (c),"D" (s),"c" (count)
        :"cx","di");
    return s;
}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.
extern inline int strcmp(char *cs, char *ct)
{
    register int __res __asm__("ax");
    __asm__("cld\n"
        "1:\tlodsb\n\t"
        "scasb\n\t"
        "jne 2f\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b\n\t"
        "xorl %%eax,%%eax\n\t"
        "jmp 3f\n"
        "2:\tmovl $1,%%eax\n\t"
        "jl 3f\n\t"
        "negl %%eax\n"
        "3:"
        :"=a" (__res):"D" (cs),"S" (ct):"si","di");
    return __res;
}

// Copy the NULL-terminated string src into dest, and
// return dest.
extern inline char *strcpy(char *dest, const char *src)
{
    __asm__("cld\n"
        "1:\tlodsb\n\t"
        "stosb\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b"
        ::"S" (src),"D" (dest):"si","di","ax");
    return dest;
}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.
extern inline char *strcat(char *dest, const char *src)
{
    __asm__("cld\n\t"
        "repne\n\t"
        "scasb\n\t"
        "decl %1\n"
        "1:\tlodsb\n\t"
        "stosb\n\t"
        "testb %%al,%%al\n\t"
        "jne 1b"
        ::"S" (src),"D" (dest),"a" (0),"c" (0xffffffff):"si","di","ax","cx");
    return dest;
}

int max(int a, int b) {
    if (a > b) return a;
    else return b;
}

int min(int a, int b) {
    if (a < b) return a;
    else return b;
}

extern inline int strlen(const char * s)
{
register int __res __asm__("cx");
__asm__("cld\n\t"
        "repne\n\t"
        "scasb\n\t"
        "notl %0\n\t"
        "decl %0"
        :"=c" (__res):"D" (s),"a" (0),"0" (0xffffffff):"di");
return __res;
}
// int strlen(char* str) {
//     int i;
//     for (i = 0; str[i] != 0; i++) {}
//     return i;
// }

