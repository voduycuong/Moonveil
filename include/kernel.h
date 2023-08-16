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
#include "test.h"

extern char *commands[];
extern char *colors[];

#define MAX_CMD_SIZE 100
#define MAX_HISTORY_SIZE 5

void cli();
void show_welcome_screen();
void show_prompt(int flag);
void show_error(char *cmd, char *error_message);

#endif