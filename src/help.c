#include "help.h"
#include "kernel.h"

// help command
void show_help(char *buffer, char flag)
{
    if (flag == 'h')
    {
        if (strcmp(subst(buffer, 5), "help")) // help command in detail
            show_help_info();

        else if (strcmp(subst(buffer, 5), "clear")) // clear command in detail
            clear_screen_info();

        else if (strcmp(subst(buffer, 5), "setcolor")) // setcolor command in detail
            set_color_info();

        else if (strcmp(subst(buffer, 5), "showinfo")) // showinfo command in detail
            show_info_info();

        else if (strcmp(subst(buffer, 5), "printf")) // printf command in detail
            printf_info();

        else if (strcmp(subst(buffer, 5), "about")) // about command in detail
            show_about_info();

        else
            show_error(subst(buffer, 5), "is not found. See 'help' to see available commands.");
    }

    else if (flag == 'x')
    {
        uart_puts("\n");
        uart_puts("\thelp\t\t\t\tShow brief information of all commands\n");
        uart_puts("\thelp <command_name>\t\tShow full information of all commands\n");
        uart_puts("\tclear\t\t\t\tClear screen\n");
        uart_puts("\tsetcolor\t\t\tSet text/background color of the console\n");
        uart_puts("\tshowinfo\t\t\tShow board revision and board MAC address\n");
        uart_puts("\tprintf\t\t\t\tPrint out data\n");
        uart_puts("\tabout\t\t\t\tShow credit\n");
        uart_puts("\n");
        uart_puts("\tHint: Use '_' and '+' to scroll down command history list.\n");
    }
}

// help command info
void show_help_info()
{
    uart_puts("\n");
    uart_puts("To show all available commands of the program.\n");
    uart_puts("Use help <command_name> to show detail of the command.\n");
    uart_puts("Example: help clear\n");
}