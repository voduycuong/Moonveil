#include "kernel.h"

void main()
{
	// Set up serial console
	uart_init();

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
	static char command_history[20][MAX_CMD_SIZE]; // Max history is 50 commands

	static int index = 0;
	static int command_index = 0;

	static char cmd_flag = 'x';

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
				command_history[command_index][i] = cli_buffer[i];
			command_index++;

			// Check whether 'help' goes with parameter or not
			static char temp[10];
			for (int i = 0; i < 5; i++) // Save the current buffer
				temp[i] = cli_buffer[i];

			if (temp[4] == ' ') // Check for space which means receive another parameter
			{
				temp[4] = '\0'; // Enclose the string
				if (strcmp(temp, "help"))
					cmd_flag = 'h'; // Turn on 'help' with parameter flag
			}
			// Clear temp
			temp[0] = '\0';

			// Check buffer with available commands
			if (strcmp(cli_buffer, "help") || cmd_flag == 'h') // help
			{
				show_help(cli_buffer, cmd_flag);
				cmd_flag = 'x';
			}

			else if (strcmp(cli_buffer, "clear")) // clear
				clear_screen();

			else if (strcmp(cli_buffer, "setcolor")) // setcolor
			{
				show_error(cli_buffer, "must go with parameter(s). See 'help setcolor'.");
				set_color(cli_buffer, cmd_flag);
			}

			else if (strcmp(cli_buffer, "showinfo")) // showinfo
				show_info();

			// else if (strcmp(cli_buffer, "printf")) // printf
			// 	printf();

			else if (strcmp(cli_buffer, "about")) // about
				show_about();

			// Show error if command not found
			else
				show_error(cli_buffer, "is not found. See 'help' to see available commands.");

			// Return to command line
			uart_puts("\n");
			show_prompt();

			// Emply the buffer
			cli_buffer[0] = '\0';
			index = 0;
		}
	}
}

// Function for showing a welcome screen when OS boot up
void show_welcome_screen()
{
	clear_screen();
	show_about(); // Welcome screen
	uart_puts("\nType 'help' to show list of available commands\n");
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
void show_error(char *cmd, char *error_message)
{
	uart_puts("\nError: '");
	uart_puts(cmd);
	uart_puts("' ");
	uart_puts(error_message);
	uart_puts("\n");
}