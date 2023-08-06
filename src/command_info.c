#include "command_info.h"
#include "uart.h"

// help command info
void show_help_info()
{
    uart_puts("\n");
    uart_puts("To show all available commands of the program.\n");
    uart_puts("Use help <command_name> to show detail of the command.\n");
    uart_puts("Example: help clear\n");
}

// clear command info
void clear_screen_info()
{
    uart_puts("\n");
    uart_puts("To clear terminal screen.\n");
    uart_puts("This is not the actual clear screen as usual.\n");
}

// setcolor command info
void set_color_info()
{
    uart_puts("\n");
    uart_puts("Set text color, and/or background color of the console to one of the following colors: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
    uart_puts("Use command with option below:\n");
    uart_puts("\t\t-t <text color>\n");
    uart_puts("\t\t-b <background color>\n");
    uart_puts("\tExample:\n");
    uart_puts("\t\tsetcolor -t yellow\n");
    uart_puts("\t\tsetcolor -b yellow -t white\n");
}

// showinfo command info
void show_info_info()
{
    uart_puts("\n");
    uart_puts("Show board revision and board MAC address.\n");
}

// printf command info
void printf_info()
{
    uart_puts("\n");
    uart_puts("printf command in detail\n");
}

// about command info
void about_info()
{
    uart_puts("\n");
    uart_puts("about command in detail\n");
}

// test command info
void test_info()
{
    uart_puts("\n");
    uart_puts("test command in detail\n");
}