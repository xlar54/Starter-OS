#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr.h"

void initialise_keyboard();
void keyboard_handler(registers_t *regs);
unsigned char getch();
unsigned char kbhit();


#endif
