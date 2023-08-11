#ifndef _PRINT_H
#define _PRINT_H

#include "./gcclib/stddef.h"
#include "./gcclib/stdint.h"
#include "./gcclib/stdarg.h"
#include "uart.h"

void printf(char *string, ...);

#endif