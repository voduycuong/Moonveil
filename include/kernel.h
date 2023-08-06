#ifndef _KERNEL_H
#define _KERNEL_H

#define MAX_CMD_SIZE 100

char *commands[] = {"help", "clear", "setcolor", "showinfo", "printf", "about", "test"};
char *color[] = {"black", "red", "green", "yellow", "blue", "purple", "cyan", "white"};

void cli();
void show_welcome_screen();
void show_prompt();

// Commands
void show_help();
void show_advanced_help(char *help_option);
void clear_screen();
void set_color();
void show_info();
void show_about();

#endif