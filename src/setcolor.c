#include "setcolor.h"

// setcolor command
void set_color(char *buffer, char flag)
{
    if (flag = 's')
    {
        uart_puts("\x1b[32mtesting\n");
    }
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