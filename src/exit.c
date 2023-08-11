#include "exit.h"

void exit()
{
    uart_puts("\033c");
}

void exit_info()
{
    uart_puts("UNDER CONSTRUCTION");
}