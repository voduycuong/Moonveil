#ifndef _SETCOLOR_H
#define _SETCOLOR_H

#include "uart.h"
#include "string.h"

void set_color(char *buffer, char flag);
void set_color_info();
void set_text_color(char *buffer, int i);
void set_foreground_color(char *buffer, int i);

#endif