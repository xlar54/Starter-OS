// monitor.h -- Defines the interface for monitor.h
//              From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define LIGHTBROWN 14
#define WHITE 15

#include "common.h"

void putch(unsigned char c);
void puts(unsigned char *);
void clrscr();
void puthex(u32int n);
void putdec(u32int n);
void setForeColor(unsigned char forecolor);
void setBackColor(unsigned char backcolor);
static void move_cursor();
static void scroll();
unsigned char getch();

#endif // MONITOR_H
