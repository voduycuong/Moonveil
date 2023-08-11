#include "setcolor.h"
#include "kernel.h"

extern int color_flag;

// setcolor command
void set_color(char *buffer, char flag)
{
    if (flag == 's')
    {
        for (int i = 0; i < strlen(buffer); i++)
        {
            if (buffer[i] == '-' && buffer[i + 1] == 't') // text color option
            {
                if (strcmp(subst(buffer, 12), colors[0])) // black
                    uart_puts("\x1b[30m");
                else if (strcmp(subst(buffer, 12), colors[1])) // red
                    uart_puts("\x1b[31m");
                else if (strcmp(subst(buffer, 12), colors[2])) // green
                    uart_puts("\x1b[32m");
                else if (strcmp(subst(buffer, 12), colors[3])) // yellow
                    uart_puts("\x1b[33m");
                else if (strcmp(subst(buffer, 12), colors[4])) // blue
                    uart_puts("\x1b[34m");
                else if (strcmp(subst(buffer, 12), colors[5])) // purple
                    uart_puts("\x1b[35m");
                else if (strcmp(subst(buffer, 12), colors[6])) // cyan
                    uart_puts("\x1b[36m");
                else if (strcmp(subst(buffer, 12), colors[7])) // white
                    uart_puts("\x1b[37m");

                else
                {
                    uart_puts("\n'");
                    uart_puts(subst(buffer, 12));
                    uart_puts("' color not exists.\n");
                }
                break;
            }
            else if (strcmp(subst(buffer, 9), "default")) // default
                color_flag = 0;
        }
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
    uart_puts("\t\tdefault (default color of the CLI)\n");

    uart_puts("\tExample:\n");
    uart_puts("\t\tsetcolor -t yellow\n");
    uart_puts("\t\tsetcolor -b yellow -t white\n");
}