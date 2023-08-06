#include "kernel.h"
#include "mbox.h"

void main()
{
	// Set up serial console
	uart_init();

	// show_welcome_screen();

	// For testing
	show_prompt();

	// Run CLI
	while (1)
	{
		cli();
	}
}

void show_welcome_screen()
{
	clear_screen();

	// Welcome screen
	show_about();

	uart_puts("\n");
	uart_puts("Type 'help' to show list of available commands\n");
	uart_puts("\n");
	show_prompt();
}

void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];
	static char command_history[50][MAX_CMD_SIZE]; // Max history is 50 commands

	static int index = 0;
	static int command_index = 0;

	// Read and send back each char
	char c = uart_getc();

	// Put into a buffer until got new line character
	if (c != '\n')
	{
		if (c != '\b')
		{
			uart_sendc(c);
			cli_buffer[index] = c; // Store into the buffer
			index++;
		}

		else if (c == '\b')
		{
			index--;
			if (index >= 0)
			{
				cli_buffer[index] = '\0';
				uart_puts("\033[1D"); // Move cursor left 1 step
				uart_puts("\033[0K"); // Clear line from cursor left
			}

			else if (index < 0)
			{
				index = 0;
			}
		}
	}

	else if (c == '\n')
	{
		// Check if no input
		if (cli_buffer[0] == '\0')
		{
			// Return to command line
			uart_puts("\n");
			show_prompt();
			index = 0;
		}
		else
		{
			cli_buffer[index] = '\0';

			// Save current buffer
			for (int i = 0; cli_buffer[i] != '\0'; i++)
			{
				command_history[command_index][i] = cli_buffer[i];
			}
			command_index++;

			// Check buffer with available commands
			if (strcmp(cli_buffer, "help"))
				show_help();

			else if (strcmp(cli_buffer, "setcolor"))
				set_color();

			else if (strcmp(cli_buffer, "showinfo"))
				show_info();

			else if (strcmp(cli_buffer, "clear"))
				clear_screen();

			else if (strcmp(cli_buffer, "about"))
				show_about();

			// Show errors if command not found
			else
			{
				uart_puts("\n");
				uart_puts(cli_buffer);
				uart_puts(": command not found\n");
			}

			// Return to command line
			uart_puts("\n");
			show_prompt();

			// Clear the buffer
			cli_buffer[0] = '\0';
			index = 0;
		}
	}
}

// help command
void show_help()
{
	uart_puts("\n");
	uart_puts("	help				Show brief information of all commands\n");
	uart_puts("	help <command_name>		Show full information of all commands\n");
	uart_puts("	clear				Clear screen\n");
	uart_puts("	set color			Set text/background color of the console\n"); //  color, and/or background
	//  to one of the following colors: BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE
	uart_puts("		-t <text color>\n");
	uart_puts("		-b <background color>\n");
	uart_puts("	showinfo			Show board revision and board MAC address\n");
	uart_puts("	printf				Print out data\n");
	uart_puts("	about				Show credit\n");
}

// clear command
void clear_screen()
{
	// "Fake" clear screen
	uart_puts("\033[2J\033[H"); // Clear entire screen + Move cursor to upper left corner
}

// setcolor command
void set_color()
{
	uart_puts("\x1b[32mtesting\n");
}

// showinfo command
void show_info()
{
	mBuf[0] = 8 * 4;		// Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
	mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
	mBuf[2] = 0x00010002;	// TAG Identifier: Get board revision
	mBuf[3] = 8;			// Value buffer size in bytes (max of request and response lengths)
	mBuf[4] = 0;			// REQUEST CODE = 0
	mBuf[5] = 3;			// clock id: ARM system clock
	mBuf[6] = 0;			// clear output buffer (response data are mBuf[5] & mBuf[6])
	mBuf[7] = MBOX_TAG_LAST;
	if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
	{
		uart_puts("Response Code for whole message: ");
		uart_hex(mBuf[1]);
		uart_puts("\n");
		uart_puts("Response Code in Message TAG: ");
		uart_hex(mBuf[4]);
		uart_puts("\n");
		uart_puts("DATA: ARM clock rate = ");
		uart_dec(mBuf[6]);
		uart_puts("\n");
	}
	else
	{
		uart_puts("Unable to query!\n");
	}
}

void show_about()
{
	uart_puts("  								*** RMIT University Vietnam ***\n");
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
}

// Function for comparing 2 strings
int strcmp(char *a, char *b)
{
	int i = 0, c = 0;
	while ((a[i] != '\0') || (b[i] != '\0'))
	{
		if (a[i] != b[i])
			c++;
		i++;
	}

	if (c == 0)
		return 1;
	else
		return 0;
}

void show_prompt()
{
	uart_puts("Moonveil> ");
}