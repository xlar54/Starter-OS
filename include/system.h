#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"

void panic(const char *message, const char *file, u32int line);
void panic_assert(const char *file, u32int line, const char *desc);

#endif // SYSTEM_H
