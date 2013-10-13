#ifndef ISR_H
#define ISR_H

//
// isr.h -- Interface and structures for high level interrupt service routines.
//          Part of this code is modified from Bran's kernel development tutorials.
//          Rewritten for JamesM's kernel development tutorials.
//

#include "common.h"

// A few defines to make life a little easier
#define IRQ0 32 // System timer
#define IRQ1 33// Keyboard (why isnt this 33 ???)
#define IRQ2 34 // Cascade interupt for IRQs 8-15
#define IRQ3 35 // COM2
#define IRQ4 36 // COM1
#define IRQ5 37 // Sound card
#define IRQ6 38 // Floppy disk controller
#define IRQ7 39 // Real-time clock 
#define IRQ8 40 // Open interrupt
#define IRQ9 41 // Open
#define IRQ10 42 // Open
#define IRQ11 43 // Open
#define IRQ12 44 // PS2 Mouse
#define IRQ13 45 // Floating point coprocessor
#define IRQ14 46 // Primary IDE
#define IRQ15 47 // Secondary IDE

typedef struct registers
{
    u32int ds;                  // Data segment selector
    u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
    u32int int_no, err_code;    // Interrupt number and error code (if applicable)
    u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void (*isr_t)(registers_t*);
void register_interrupt_handler(u8int n, isr_t handler);

#endif
