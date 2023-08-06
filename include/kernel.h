#ifndef _KERNEL_H
#define _KERNEL_H

#include "uart.h"

#define MAX_CMD_SIZE 100
char *commands[] = {"help", "clear", "setcolor", "showinfo", "printf", "about", "test"};

void show_welcome_screen();
void cli();

// Commands
void show_help();
void clear_screen();
void set_color();
void show_info();
void show_about();

int strcmp(char *a, char *b);
char *subst(char *source, int pos);
void show_prompt();

#endif