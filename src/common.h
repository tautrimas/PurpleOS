// common.h -- Defines typedefs and some global functions.
//             From JamesM's kernel development tutorials.

#ifndef COMMON_H
#define COMMON_H

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

int max(int a, int b);
int min(int a, int b);
int strlen(const char * s);

// Write len copies of val into dest.
// extern inline void* memset(void * s,int c,int count)
// {
//     __asm__("cld\n\t"
//         "rep\n\t"
//         "stosb"
//         ::"a" (c),"D" (s),"c" (count)
//         :"cx","di");
//     return s;
// }
void* memset(void *dest, int val, unsigned int len);

//Copy len bytes from src to dest.
//extern inline void* memcpy(u8int *dest, const u8int *src, u32int n)
//{
//    __asm__("cld\n\t"
//        "rep\n\t"
//        "movsb"
//        ::"c" (n),"S" (src),"D" (dest)
//        :"cx","si","di");
//    return dest;
//}
void* memcpy(u8int *dest, const u8int *src, u32int len);

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

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);

#endif // COMMON_H
