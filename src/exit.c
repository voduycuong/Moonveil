#include "exit.h"
#include "uart.h"

void exit()
{
    uart_puts("\033c");
}

void exit_info()
{
}