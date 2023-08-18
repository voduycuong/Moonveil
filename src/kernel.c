#include "kernel.h"

char *commands[] = {"help", "clear", "setcolor", "showinfo", "about", "test"};
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
	static char cmd_history[MAX_HISTORY_SIZE][MAX_CMD_SIZE]; // Max history is 20 commands

	static int index = 0;	  // For indexing elements in cli_buffer
	static int cmd_index = 0; // For indexing commands in history

	static int cmd_history_length = 0; // Count history size
	static int underline_count = 0;	   // Count underline key
	static int plus_count = 0;		   // Count plus key

	static char cmd_option = 'x'; // Command option flag
	static int spec_pressed = 0;  // Special key: '_', '+', '\\t' flag

	char c = uart_getc(); // Read and send back each char

	// '_' key is pressed
	if (c == '_')
	{
		spec_pressed = 1;
		if (cmd_index > 0) // Reach the start of history
		{
			underline_count++;

			// Check if '+' is pressed, then eliminate
			if (plus_count > 0)
				plus_count--;

			cmd_index--; // Go back to previous command

			// Load command from history into current buffer
			for (int i = 0; i < strlen(cmd_history[cmd_index]); i++)
			{
				cli_buffer[i] = cmd_history[cmd_index][i];
				cli_buffer[i + 1] = '\0';
			}

			// Clear previous command shown on terminal
			for (int i = 0; i < strlen(cmd_history[cmd_index + 1]); i++)
				uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer
			uart_puts("\033[0K");	  // Clear line from cursor right

			uart_puts(cli_buffer); // Show buffer
		}
	}

	// '+' key is pressed
	else if (c == '+')
	{
		spec_pressed = 1;
		if (cmd_index < cmd_history_length - 1) // Reach the end of history
		{
			plus_count++;

			// Check if '-' is pressed, then eliminate
			if (underline_count > 0)
				underline_count--;

			cmd_index++; // Go to next command

			// Load command from history into current buffer
			for (int i = 0; i < strlen(cmd_history[cmd_index]); i++)
			{
				cli_buffer[i] = cmd_history[cmd_index][i];
				cli_buffer[i + 1] = '\0';
			}

			// Clear previous command shown on terminal
			for (int i = 0; i < strlen(cmd_history[cmd_index - 1]); i++)
				uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer
			uart_puts("\033[0K");	  // Clear line from cursor right

			uart_puts(cli_buffer); // Show buffer
		}
	}

	// Tab key is pressed
	else if (c == '\t')
	{
		spec_pressed = 1;

		int found_index = 0; // Indexing the found command
		for (int i = 0; i < 5; i++)
			if (strsearch(commands[i], cli_buffer))
			{
				found_index = i;
				break;
			}

		// Clear previous command shown on terminal
		for (int i = 0; i < strlen(cli_buffer); i++)
			uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer
		uart_puts("\033[0K");	  // Clear line from cursor right

		// Load found command into current buffer
		for (int i = 0; i < strlen(commands[found_index]); i++)
		{
			cli_buffer[i] = commands[found_index][i];
			cli_buffer[i + 1] = '\0';
		}

		uart_puts(cli_buffer); // Show buffer
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
			if (spec_pressed)
				index = strlen(cli_buffer);
			index--;

			if (index >= 0)
			{
				// Clear 1 char in buffer
				cli_buffer[index] = '\0';

				// Clear 1 char in terminal
				uart_puts("\033[1D\033[0K"); // Move cursor left 1 step & Clear line from cursor left
			}

			// Restart index if buffer is empty
			else if (index < 0)
				index = 0;
		}
	}

	// Return key is pressed
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
			if (spec_pressed)
				cmd_index = cmd_history_length; // Back to top

			// Save current buffer
			for (int i = 0; i < strlen(cli_buffer); i++)
			{
				cmd_history[cmd_index][i] = cli_buffer[i];
				cmd_history[cmd_index][i + 1] = '\0';
			}

			cmd_index++;
			cmd_history_length++;

			// Reset command index if exceeded
			if (cmd_index >= MAX_HISTORY_SIZE)
				cmd_index %= MAX_HISTORY_SIZE;
			else if (cmd_index < 0)
				cmd_index = cmd_history_length;

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

			else if (strcmp(cli_buffer, commands[5])) // test
			{
				// uart_puts("\nTesting printf command: ");
				// uart_puts("\n-----------------------------------------------");
				// test("test_printf");

				// uart_puts("\n");

				// uart_puts("\nTesting mailbox setup: ");
				// uart_puts("\n-----------------------------------------------");
				// test("test_mailbox");

				for (int i = 0; i <= cmd_history_length; i++)
				{
					uart_puts("\n[");
					uart_dec(i);
					uart_puts("] = ");
					uart_puts(cmd_history[i]);
				}
			}

			// Show error if command not found
			else
				show_error(cli_buffer, "is not found. See 'help' to see available commands.");

			// Return to command line
			uart_puts("\n");
			show_prompt(color_flag);

			// Flush the buffer
			for (int i = 0; i < MAX_CMD_SIZE; i++)
				cli_buffer[i] = '\0';
			index = 0;

			underline_count = 0; // Reset the count for command history
			plus_count = 0;		 // Reset the count for command history
			spec_pressed = 0;
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