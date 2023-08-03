#include "uart.h"
#define MAX_CMD_SIZE 100

void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];
	static int index = 0;

	int flag = 1;

	while (flag)
	{
		uart_puts("Moonveil > ");
		flag = 0;
	}

	// read and send back each char
	char c = uart_getc();
	uart_sendc(c);

	// put into a buffer until got new line character
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

		/* Compare with supported commands and execute
		 * ........................................... */

		// Return to command line
		index = 0;

		flag = 1;
	}
}

void main()
{
	// Set up serial console
	uart_init();

	// // Welcome screen
	// uart_puts("  												*** EEET2490 ***\n");
	// uart_puts("     									  	  	CC: Mr. Linh Tran - TA: Mr. Phuc Nguyen\n");

	// uart_puts("                           	     /\\\\\\\\            /\\\\\\\\                                                                             /\\\\\\\\\\\\            \n");
	// uart_puts("                           	     \\/\\\\\\\\\\\\        /\\\\\\\\\\\\                                                                            \\////\\\\\\           \n");
	// uart_puts("                           	      \\/\\\\\\//\\\\\\    /\\\\\\//\\\\\\                                                                       /\\\\\\    \\/\\\\\\          \n");
	// uart_puts("                           	       \\/\\\\\\\\///\\\\\\/\\\\\\/ \\/\\\\\\     /\\\\\\\\\\        /\\\\\\\\\\     /\\\\/\\\\\\\\\\\\    /\\\\\\    /\\\\\\   /\\\\\\\\\\\\\\\\  \\///     \\/\\\\\\         \n");
	// uart_puts("                           	        \\/\\\\\\  \\///\\\\\\/   \\/\\\\\\   /\\\\\\///\\\\\\    /\\\\\\///\\\\\\  \\/\\\\\\////\\\\\\  \\//\\\\\\  /\\\\\\  /\\\\\\/////\\\\\\  /\\\\\\    \\/\\\\\\        \n");
	// uart_puts("                           	         \\/\\\\\\    \\///     \\/\\\\\\  /\\\\\\  \\//\\\\\\  /\\\\\\  \\//\\\\\\ \\/\\\\\\  \\//\\\\\\  \\//\\\\\\/\\\\\\  /\\\\\\\\\\\\\\\\\\\\\\  \\/\\\\\\    \\/\\\\\\       \n");
	// uart_puts("                           	          \\/\\\\\\             \\/\\\\\\ \\//\\\\\\  /\\\\\\  \\//\\\\\\  /\\\\\\  \\/\\\\\\   \\/\\\\\\   \\//\\\\\\\\\\  \\//\\\\///////   \\/\\\\\\    \\/\\\\\\      \n");
	// uart_puts("                           	           \\/\\\\\\             \\/\\\\\\  \\///\\\\\\\\\\/    \\///\\\\\\\\\\/   \\/\\\\\\   \\/\\\\\\    \\//\\\\\\    \\//\\\\\\\\\\\\\\\\\\\\ \\/\\\\\\  /\\\\\\\\\\\\\\\\\\  \n");
	// uart_puts("                           	            \\///              \\///     \\/////        \\/////     \\///    \\///      \\///      \\//////////  \\///  \\/////////  \n");
	// uart_puts("\n");
	// uart_puts("     										Developed by Vo Duy Cuong - S3941544\n");

	uart_puts("---> The welcome screen\n");

	// Run CLI
	while (1)
	{
		cli();
	}
}
