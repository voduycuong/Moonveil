#include "kernel.h"
#include "mbox.h"
#include "uart.h"
// #include "printf.h"
#include "command_info.h"
#include "string.h"

#define DEBUG_MODE = 1
#ifdef DEBUG_MODE
debugging = 1;
#endif

void main()
{
	// Set up serial console
	uart_init();

	if (debugging)
		show_prompt();
	else
		show_welcome_screen();

	// Run CLI
	while (1)
	{
		cli();
	}
}

void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];
	static char command_history[50][MAX_CMD_SIZE]; // Max history is 50 commands

	static int index = 0;
	static int command_index = 0;

	static char option_flag;
	static char temp[10];

	// Read and send back each char
	char c = uart_getc();

	// Put into a buffer until got new line character
	if (c != '\n')
	{
		// Check for backspace, if not, continue bufferring
		if (c != '\b')
		{
			uart_sendc(c);
			cli_buffer[index] = c; // Store into the buffer
			index++;
		}

		// If backspaced, clear in buffer + clear on terminal
		else if (c == '\b')
		{
			index--;
			if (index >= 0)
			{
				// Clear 1 char in buffer
				cli_buffer[index] = '\0';

				// Clear in terminal
				uart_puts("\033[1D"); // Move cursor left 1 step
				uart_puts("\033[0K"); // Clear line from cursor left
			}

			// Restart index if buffer is empty
			else if (index < 0)
			{
				index = 0;
			}
		}
	}

	// Auto complete

	// User input 'return'
	else if (c == '\n')
	{
		for (int i = 0; i < 5; i++)
			temp[i] = cli_buffer[i];

		if (temp[4] == ' ')
		{
			temp[4] = '\0';
			uart_puts(temp);
			uart_puts("\n");
			if (strcmp(temp, commands[0]))
				option_flag = 'h';
			else
				option_flag = 'x';
		}

		uart_puts("Option flag is ");
		uart_puts(option_flag);
		uart_puts("\n");

		// Check if none command
		if (cli_buffer[0] == '\0')
		{
			// Return to command line
			uart_puts("\n");
			show_prompt();
			index = 0;
		}

		else
		{
			// Command is complete
			cli_buffer[index] = '\0';

			// Save current buffer
			for (int i = 0; cli_buffer[i] != '\0'; i++)
			{
				command_history[command_index][i] = cli_buffer[i];
			}
			command_index++;

			if (option_flag == 'h')
			{
				uart_puts(option_flag);
			}

			// show_advanced_help(subst(cli_buffer, 5));

			// Check buffer with available commands
			if (strcmp(cli_buffer, commands[0])) // help
				show_help();

			else if (strcmp(cli_buffer, commands[1])) // clear
				clear_screen();

			else if (strcmp(cli_buffer, commands[2])) // setcolor
				set_color();

			else if (strcmp(cli_buffer, commands[3])) // showinfo
				show_info();

			// else if (strcmp(cli_buffer, commands[4])) // printf
			// 	printf();

			else if (strcmp(cli_buffer, commands[5])) // about
				show_about();

			// else if (strcmp(cli_buffer, commands[6])) // <--------------------------- For testing
			// {
			// 	uart_puts("\n");
			// 	uart_puts(strcmp(subst(cli_buffer, 5), commands[0]));
			// }

			// Show error if command not found
			else
			{
				show_error(cli_buffer);
			}

			// Return to command line
			uart_puts("\n");
			show_prompt();

			// Emply the buffer
			cli_buffer[0] = '\0';
			index = 0;
		}
	}
}

// help command
void show_help()
{
	uart_puts("\n");
	uart_puts("	help\t\t\t\tShow brief information of all commands\n");
	uart_puts("	help <command_name>\t\tShow full information of all commands\n");
	uart_puts("	clear\t\t\t\tClear screen\n");
	uart_puts("	setcolor\t\t\tSet text/background color of the console\n");
	uart_puts("	showinfo\t\t\tShow board revision and board MAC address\n");
	uart_puts("	printf\t\t\t\tPrint out data\n");
	uart_puts("	about\t\t\t\tShow credit\n");
}

// "advanced" help command
void show_advanced_help(char *help_option)
{
	if (strcmp(help_option, commands[0])) // help command in detail
		show_help_info();

	else if (strcmp(help_option, commands[1])) // clear command in detail
		clear_screen_info();

	else if (strcmp(help_option, commands[2])) // setcolor command in detail
		set_color_info();

	else if (strcmp(help_option, commands[3])) // showinfo command in detail
		show_info_info();

	else if (strcmp(help_option, commands[4])) // printf command in detail
		printf_info();

	else if (strcmp(help_option, commands[5])) // about command in detail
		about_info();

	else if (strcmp(help_option, commands[6])) // test command in detail
		test_info();
}

// clear command
void clear_screen()
{
	// "Fake" clear screen
	uart_puts("\033[2J\033[f"); // Clear entire screen + Move cursor to upper left corner
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
	uart_puts("\t\t\t\t\t\t\t\t\t*** RMIT University Vietnam ***\n");
	uart_puts("\t\t\t\t\t\t\t\t*** EEET2490 - Embedded Systems: Operating Systems & Interfacing ***\n");
	uart_puts("\t\t\t\t\t\t\t\t\t\tCC: Mr. Linh Tran - TA: Mr. Phuc Nguyen\n");

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

	uart_puts("\t\t\t\t\t\t\t\t\t\tDeveloped by Vo Duy Cuong - S3941544\n");
}

// Function for showing a welcome screen when OS boot up
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

// Function for showing prompt at the beginning of each command
void show_prompt()
{
	uart_puts("\x1b[1;34mMoonveil> "); // Bold & Blue foreground text
	uart_puts("\x1b[0m");			   // Set as default
}

// Function for output an error message
void show_error(char *errorMessage)
{
	uart_puts("\n");
	uart_puts("Error: '");
	uart_puts(errorMessage);
	uart_puts("' command is not found. See 'help'.\n");
}