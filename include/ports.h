#ifndef PORTS_H
#define PORTS_H

#include "common.h"

void outportb(u16int port, u8int value);

void outportw(u16int port, u16int value);

u8int inportb(u16int port);

u16int inportw(u16int port);

#endif // PORTS_H

