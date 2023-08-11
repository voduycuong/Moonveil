#include "setcolor.h"

extern int color_flag;
extern char *colors[];

// setcolor command
void set_color(char *buffer, char flag)
{
    if (flag == 's')
    {
        int i = 0;

        if (strcmp(subst(buffer, 9), "default")) // default
            color_flag = 0;

        for (i = strlen(buffer); i > 0; i--)
        {
            if (buffer[i] == 't' && buffer[i - 1] == '-') // text color option
            {
                set_text_color(buffer, i);
                break;
            }
            else if (buffer[i] == 'b' && buffer[i - 1] == '-') // text color option
            {
                set_foreground_color(buffer, i);
                break;
            }
        }

        buffer[i - 2] = '\0';

        for (i = strlen(buffer); i > 0; i--)
        {
            if (buffer[i] == 'b' && buffer[i - 1] == '-') // text color option
            {
                set_foreground_color(buffer, i);
                break;
            }
            else if (buffer[i] == 't' && buffer[i - 1] == '-') // text color option
            {
                set_text_color(buffer, i);
                break;
            }
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

    uart_puts("\tUse command with option below:\n");
    uart_puts("\t\t-t <text color>\n");
    uart_puts("\t\t-b <background color>\n");
    uart_puts("\t\tdefault (default color of the CLI)\n");

    uart_puts("\tExample:\n");
    uart_puts("\t\tsetcolor -t yellow\n");
    uart_puts("\t\tsetcolor -b yellow -t white\n");
}

void set_text_color(char *buffer, int i)
{
    if (strcmp(subst(buffer, i + 2), colors[0])) // black
        uart_puts("\x1b[30m");
    else if (strcmp(subst(buffer, i + 2), colors[1])) // red
        uart_puts("\x1b[31m");
    else if (strcmp(subst(buffer, i + 2), colors[2])) // green
        uart_puts("\x1b[32m");
    else if (strcmp(subst(buffer, i + 2), colors[3])) // yellow
        uart_puts("\x1b[33m");
    else if (strcmp(subst(buffer, i + 2), colors[4])) // blue
        uart_puts("\x1b[34m");
    else if (strcmp(subst(buffer, i + 2), colors[5])) // purple
        uart_puts("\x1b[35m");
    else if (strcmp(subst(buffer, i + 2), colors[6])) // cyan
        uart_puts("\x1b[36m");
    else if (strcmp(subst(buffer, i + 2), colors[7])) // white
        uart_puts("\x1b[37m");
    else
    {
        uart_puts("\n'");
        uart_puts(subst(buffer, i + 2));
        uart_puts("' color not exists.\n");
    }
}

void set_foreground_color(char *buffer, int i)
{

    if (strcmp(subst(buffer, i + 2), colors[0])) // black
        uart_puts("\x1b[40m");
    else if (strcmp(subst(buffer, i + 2), colors[1])) // red
        uart_puts("\x1b[41m");
    else if (strcmp(subst(buffer, i + 2), colors[2])) // green
        uart_puts("\x1b[42m");
    else if (strcmp(subst(buffer, i + 2), colors[3])) // yellow
        uart_puts("\x1b[43m");
    else if (strcmp(subst(buffer, i + 2), colors[4])) // blue
        uart_puts("\x1b[44m");
    else if (strcmp(subst(buffer, i + 2), colors[5])) // purple
        uart_puts("\x1b[45m");
    else if (strcmp(subst(buffer, i + 2), colors[6])) // cyan
        uart_puts("\x1b[46m");
    else if (strcmp(subst(buffer, i + 2), colors[7])) // white
        uart_puts("\x1b[47m");
    else
    {
        uart_puts("\n'");
        uart_puts(subst(buffer, i + 2));
        uart_puts("' color not exists.\n");
    }
}