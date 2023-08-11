#include "setcolor.h"
#include "kernel.h"

// setcolor command
void set_color(char *buffer, char flag)
{
    if (flag == 's')
    {
        if (strcmp(subst(buffer, 5), "help")) // help command in detail
        {
        }
        // uart_puts("\x1b[32mtesting\n");
    }
    else if (flag == 'x')
        uart_puts("\n'setcolor' must go with parameter(s). See 'help setcolor'.");
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