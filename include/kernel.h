#include "uart.h"

#define MAX_CMD_SIZE 100

void show_welcome_screen();
void cli();

// Commands
void help();
void clear();
void setcolor();
void showinfo();