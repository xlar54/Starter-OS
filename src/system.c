#include "system.h"

void panic(const char *message, const char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    puts("PANIC(");
    puts(message);
    puts(") at ");
    puts(file);
    puts(":");
    putdec(line);
    puts("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

void panic_assert(const char *file, u32int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    puts("ASSERTION-FAILED(");
    puts(desc);
    puts(") at ");
    puts(file);
    puts(":");
    putdec(line);
    puts("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
