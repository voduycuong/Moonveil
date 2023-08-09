#include "clear.h"

// clear command
void clear_screen()
{
    uart_puts("\033[2J\033[f"); // Clear entire screen + Move cursor to upper left corner
}

// clear command info
void clear_screen_info()
{
    uart_puts("\n");
    uart_puts("To clear terminal screen.\n");
    uart_puts("This is not the actual clear screen as usual.\n");
}