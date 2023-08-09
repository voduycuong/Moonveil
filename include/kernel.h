#ifndef _KERNEL_H
#define _KERNEL_H

#include "mbox.h"
#include "uart.h"
#include "string.h"
#include "help.h"
#include "clear.h"
#include "setcolor.h"
#include "showinfo.h"
#include "about.h"
#include "printf.h"

#define MAX_CMD_SIZE 100

void cli();
void show_welcome_screen();
void show_prompt();
void show_error(char *cmd, char *error_message);

#endif