#include "kernel.h"

void main()
{
	// Set up serial console
	uart_init();

	// show_welcome_screen();

	uart_puts("Moonveil> ");

	// Run CLI
	while (1)
	{
		cli();
	}
}

void show_welcome_screen()
{
	// Welcome screen
	uart_puts("  											*** RMIT University Vietnam ***\n");
	uart_puts("  								*** EEET2490 - Embedded Systems: Operating Systems & Interfacing ***\n");
	uart_puts("     									  	  CC: Mr. Linh Tran - TA: Mr. Phuc Nguyen\n");

	uart_puts("                           	     /\\\\\\\\            /\\\\\\\\                                                                             /\\\\\\\\\\\\            \n");
	uart_puts("                           	     \\/\\\\\\\\\\\\        /\\\\\\\\\\\\                                                                            \\////\\\\\\           \n");
	uart_puts("                           	      \\/\\\\\\//\\\\\\    /\\\\\\//\\\\\\                                                                       /\\\\\\    \\/\\\\\\          \n");
	uart_puts("                           	       \\/\\\\\\\\///\\\\\\/\\\\\\/ \\/\\\\\\     /\\\\\\\\\\        /\\\\\\\\\\     /\\\\/\\\\\\\\\\\\    /\\\\\\    /\\\\\\   /\\\\\\\\\\\\\\\\  \\///     \\/\\\\\\         \n");
	uart_puts("                           	        \\/\\\\\\  \\///\\\\\\/   \\/\\\\\\   /\\\\\\///\\\\\\    /\\\\\\///\\\\\\  \\/\\\\\\////\\\\\\  \\//\\\\\\  /\\\\\\  /\\\\\\/////\\\\\\  /\\\\\\    \\/\\\\\\        \n");
	uart_puts("                           	         \\/\\\\\\    \\///     \\/\\\\\\  /\\\\\\  \\//\\\\\\  /\\\\\\  \\//\\\\\\ \\/\\\\\\  \\//\\\\\\  \\//\\\\\\/\\\\\\  /\\\\\\\\\\\\\\\\\\\\\\  \\/\\\\\\    \\/\\\\\\       \n");
	uart_puts("                           	          \\/\\\\\\             \\/\\\\\\ \\//\\\\\\  /\\\\\\  \\//\\\\\\  /\\\\\\  \\/\\\\\\   \\/\\\\\\   \\//\\\\\\\\\\  \\//\\\\///////   \\/\\\\\\    \\/\\\\\\      \n");
	uart_puts("                           	           \\/\\\\\\             \\/\\\\\\  \\///\\\\\\\\\\/    \\///\\\\\\\\\\/   \\/\\\\\\   \\/\\\\\\    \\//\\\\\\    \\//\\\\\\\\\\\\\\\\\\\\ \\/\\\\\\  /\\\\\\\\\\\\\\\\\\  \n");
	uart_puts("                           	            \\///              \\///     \\/////        \\/////     \\///    \\///      \\///      \\//////////  \\///  \\/////////  \n");
	uart_puts("\n");

	uart_puts("     										Developed by Vo Duy Cuong - S3941544\n");
	uart_puts("\n");
}

void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];
	static char buffer_history[50][MAX_CMD_SIZE];
	static int index = 0;

	// read and send back each char
	char c = uart_getc();
	uart_sendc(c);

	// Put into a buffer until got new line character
	if (c != '\n')
	{
		cli_buffer[index] = c; // Store into the buffer
		index++;
	}

	else if (c == '\n')
	{
		cli_buffer[index] = '\0';

		uart_puts("\nGot commands: ");
		uart_puts(cli_buffer);
		uart_puts("\n");

		// Compare with supported commands and execute

		// Return to command line
		uart_puts("Moonveil> ");
	}
	// Clear the buffer
	cli_buffer[0] = '\0';
}

void help()
{
}

void clear()
{
}

void setcolor()
{
}

void showinfo()
{
}