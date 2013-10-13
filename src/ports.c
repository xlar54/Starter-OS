#include "ports.h"

// Write a byte out to the specified port.
void outportb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

void outportw(u16int port, u16int value)
{
   asm volatile ("outw %%ax,%%dx"::"d" (port), "a" (value));
}


u8int inportb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inportw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
