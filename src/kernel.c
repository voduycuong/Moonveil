#include "kernel.h"

char *commands[] = {"help", "clear", "setcolor", "showinfo", "about", "exit", "test"};
char *colors[] = {"black", "red", "green", "yellow", "blue", "purple", "cyan", "white"};
int color_flag = 0;

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
	static char cmd_history[20][MAX_CMD_SIZE]; // Max history is 20 commands

	static int index = 0;
	static int cmd_index = 0;
	static int cmd_history_length = 0;

	static char cmd_option = 'x';
	static int underline_count = 0;
	static int plus_count = 0;

	// Reset command index if exceeded
	if (cmd_index > 20)
		cmd_index = 0;

	// Read and send back each char
	char c = uart_getc();

	while (cmd_history[cmd_history_length][0] != '\0')
		cmd_history_length++;

	if (c == '_')
	{
		if (underline_count < cmd_history_length)
		{
			underline_count++;
			cli_buffer[0] = '\0';

			if (cmd_index > 0)
			{
				cmd_index--; // Go back to previous command

				// Load command from history into current buffer
				for (int i = 0; i < strlen(cmd_history[cmd_index]); i++)
				{
					cli_buffer[i] = cmd_history[cmd_index][i];
					cli_buffer[i + 1] = '\0';
				}
				// Check if user continue to scroll history ('_' is pressed more than once)
				for (int i = 0; i < strlen(cmd_history[cmd_index + 1]); i++)
					uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer
				uart_puts("\033[0K");	  // Clear line from cursor right
				uart_puts(cli_buffer);	  // Show buffer
			}
		}
	}

	else if (c == '+')
	{
		if (plus_count < cmd_history_length)
		{
			plus_count++;
			cli_buffer[0] = '\0';

			if (cmd_index < cmd_history_length)
			{
				cmd_index++; // Go to next command

				// Load command from history into current buffer
				for (int i = 0; i < strlen(cmd_history[cmd_index]); i++)
				{
					cli_buffer[i] = cmd_history[cmd_index][i];
					cli_buffer[i + 1] = '\0';
				}
				// Check if user continue to scroll history ('_' is pressed more than once)
				for (int i = 0; i < strlen(cmd_history[cmd_index - 1]); i++)
					uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer

				uart_puts("\033[0K");  // Clear line from cursor right
				uart_puts(cli_buffer); // Show buffer
			}
		}
	}

	// Put into a buffer until got new line character
	else if (c != '\n')
	{
		if (c == '\t')
		{
			if (cli_buffer[0] != '\0')
			{
				int p, q, r;
				for (p = 0; p < 6; p++)
					if (strsearch(commands[p], cli_buffer))
						break;

				for (r = 0; r < strlen(cli_buffer); r++)
					uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer
				uart_puts("\033[0K");	  // Clear line from cursor right

				for (q = 0; q < strlen(commands[p]); q++)
					cli_buffer[q] = commands[p][q];

				cli_buffer[strlen(commands[p])] = '\0';

				uart_puts(cli_buffer);
			}
		}

		// Check for backspace, if not, continue bufferring
		else if (c != '\b')
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
			show_prompt(color_flag);
			index = 0;
		}

		else
		{
			if (index != 0)
				// Command is complete
				cli_buffer[index] = '\0';

			cmd_index += underline_count; // Back to top top result in history
			cmd_index -= plus_count;	  // Back to top top result in history

			// Save current buffer
			for (int i = 0; i < strlen(cli_buffer); i++)
				cmd_history[cmd_index][i] = cli_buffer[i];
			cmd_index++;

			// Check whether command goes with parameter or not
			static char temp[MAX_CMD_SIZE];
			for (int i = 0; i < strlen(cli_buffer); i++) // Save the current buffer
				temp[i] = cli_buffer[i];

			if (temp[4] == ' ') // Check for space which means receive another parameter
			{
				temp[4] = '\0'; // Enclose the string
				if (strcmp(temp, commands[0]))
					cmd_option = 'h'; // Turn on 'help' with parameter flag
			}
			else if (temp[8] == ' ') // Check for space which means receive another parameter
			{
				temp[8] = '\0'; // Enclose the string
				if (strcmp(temp, commands[2]))
				{
					color_flag = 1;	  // Color flag on to bypass default prompt color
					cmd_option = 's'; // Turn on 'help' with parameter flag
				}
			}
			// Clear temp
			temp[0] = '\0';

			// Check buffer with available commands
			if (strcmp(cli_buffer, commands[0]) || cmd_option == 'h') // help
			{
				show_help(cli_buffer, cmd_option);
				cmd_option = 'x';
			}

			else if (strcmp(cli_buffer, commands[1])) // clear
				clear_screen();

			else if (strcmp(cli_buffer, commands[2]) || cmd_option == 's') // setcolor
			{
				set_color(cli_buffer, cmd_option);
				cmd_option = 'x';
			}

			else if (strcmp(cli_buffer, commands[3])) // showinfo
				show_info();

			else if (strcmp(cli_buffer, commands[4])) // about
				show_about();

			else if (strcmp(cli_buffer, commands[5])) // exit
				exit();

			else if (strcmp(cli_buffer, commands[6])) // test
			{
				// for (int i = 0; i < 20; i++)
				// {
				// 	uart_puts("\n[");
				// 	uart_dec(i);
				// 	uart_puts("] = ");
				// 	uart_puts(cmd_history[i]);
				// }

				printf(" %d", cmd_index);
			}

			// Show error if command not found
			else
				show_error(cli_buffer, "is not found. See 'help' to see available commands.");

			// Return to command line
			uart_puts("\n");
			show_prompt(color_flag);

			// Emply the buffer
			cli_buffer[0] = '\0';
			index = 0;

			underline_count = 0; // Reset the count for command history
			plus_count = 0;		 // Reset the count for command history
		}
	}
}

// Function for showing a welcome screen when OS boot up
void show_welcome_screen()
{
	show_about(); // Welcome screen
	uart_puts("\n\tType 'help' to show list of available commands\n");
	uart_puts("\n");
	show_prompt(0);
}

// Function for showing prompt at the beginning of each command
void show_prompt(int flag)
{
	if (flag == 0)
	{
		uart_puts("\x1b[0m");			   // Return to default
		uart_puts("\x1b[1;34mMoonveil> "); // Bold & Blue foreground text
		uart_puts("\x1b[0m");			   // Return to default
	}
	else
		uart_puts("Moonveil> ");
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