#include "kernel.h"

void main()
{
	// Set up serial console
	uart_init();

	clear_screen();
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
	static char command_history[20][MAX_CMD_SIZE]; // Max history is 20 commands

	static int index = 0;
	static int command_index = 0;
	static int cmd_history_length = 0;

	static char cmd_flag = 'x';

	static int history_index = 0;

	// Reset command index if exceeded
	if (command_index > 20)
		command_index = 0;

	// Read and send back each char
	char c = uart_getc();

	while (command_history[cmd_history_length][0] != '\0')
		cmd_history_length++;

	if (c == '_')
	{
		uart_puts("\nHISTORY INDEX = ");
		uart_dec(history_index);
		uart_puts("\nCOMMAND INDEX = ");
		uart_dec(command_index);
		uart_puts("\n");
		uart_puts("OUTPUT: ");

		if (command_index <= cmd_history_length)
		{
			cli_buffer[0] = '\0';

			if (command_index > 0)
			{
				command_index--; // Go back to previous command

				// Load command from history into current buffer
				for (int i = 0; i < strlen(command_history[command_index]); i++)
				{
					cli_buffer[i] = command_history[command_index][i];
					cli_buffer[i + 1] = '\0';
				}

				// Check if user continue to scroll history ('_' is pressed more than once)
				if (command_index > 0)
					for (int i = 0; i < strlen(command_history[command_index + 1]); i++)
						uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer

				uart_puts("\033[0K");  // Clear line from cursor right
				uart_puts(cli_buffer); // Show buffer
			}
		}
		else
			history_index = command_index;
	}

	// Put into a buffer until got new line character
	else if (c != '\n')
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
				uart_puts("\033[1D\033[0K"); // Move cursor left 1 step & Clear line from cursor left
			}

			// Restart index if buffer is empty
			else if (index < 0)
				index = 0;
		}
	}

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
			if (index != 0)
				// Command is complete
				cli_buffer[index] = '\0';

			// Save current buffer
			for (int i = 0; i < strlen(cli_buffer); i++)
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

			else if (strcmp(cli_buffer, "setcolor") || cmd_flag == 't' || cmd_flag == 'b') // setcolor
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

			else if (strcmp(cli_buffer, "test")) // about
			{
				for (int i = 0; i < 20; i++)
				{
					uart_puts("\n[");
					uart_dec(i);
					uart_puts("] = ");
					uart_puts(command_history[i]);
				}
			}

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
	show_about(); // Welcome screen
	uart_puts("\nType 'help' to show list of available commands\n");
	uart_puts("\n");
	show_prompt();
}

// Function for showing prompt at the beginning of each command
void show_prompt()
{
	uart_puts("\x1b[1;34mMoonveil> "); // Bold & Blue foreground text
	uart_puts("\x1b[0m");			   // Return to default
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