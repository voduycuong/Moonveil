#ifndef _KERNEL_H
#define _KERNEL_H

#include "mbox.h"
#include "uart.h"
#include "string.h"

#include "help.h"
#include "clear.h"
#include "setcolor.h"
#include "showinfo.h"
#include "printf.h"
#include "about.h"
#include "exit.h"

extern char *commands[];

#define MAX_CMD_SIZE 100

extern void cli();
void show_welcome_screen();
void show_prompt();
void show_error(char *cmd, char *error_message);

#endif