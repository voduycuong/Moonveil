#include "kernel.h"

char *commands[] = {"help", "clear", "setcolor", "showinfo", "about", "test"};
char *colors[] = {"black", "red", "green", "yellow", "blue", "purple", "cyan", "white"};
int color_flag = 0; // Flag for using the default color of prompt

void main()
{
	uart_init();		   // Set up serial console
	clear_screen();		   // Clear screen
	show_welcome_screen(); // Show welcome screen

	// Run CLI
	while (1)
	{
		cli();
	}
}

void cli()
{
	volatile uint32_t __attribute__((aligned(16))) mBuf[36];

	static char cli_buffer[MAX_CMD_SIZE];					 // CLI BUFFER
	static char cmd_history[MAX_HISTORY_SIZE][MAX_CMD_SIZE]; // COMMAND HISTORY

	static int index = 0;	  // Indexing elements in cli_buffer
	static int cmd_index = 0; // Indexing commands in history

	static int cmd_history_length = 0; // Count history size
	static int underline_count = 0;	   // Count underline key
	static int plus_count = 0;		   // Count plus key

	static char cmd_option = 'x'; // Command option flag

	char input = uart_getc(); // Get input from keyboard

	// '_' key is pressed
	if (input == '_')
	{
		if (cmd_index > 0) // Reach the start of history
		{
			underline_count++;

			// Check if '+' is pressed, then eliminate
			if (plus_count > 0)
				plus_count--;

			cmd_index--;							  // Back to previous command
			feed(cmd_history[cmd_index], cli_buffer); // Load command from history into current buffer
			clear_cmd(cmd_history[cmd_index + 1]);	  // Clear previous command shown on terminal
			uart_puts(cli_buffer);					  // Show command
			index = strlen(cli_buffer);				  // Update index
		}
	}

	// '+' key is pressed
	else if (input == '+')
	{
		if (cmd_index < cmd_history_length - 1) // Reach the end of history
		{
			plus_count++;

			// Check if '-' is pressed, then eliminate
			if (underline_count > 0)
				underline_count--;

			cmd_index++;							  // Next command
			feed(cmd_history[cmd_index], cli_buffer); // Load command from history into current buffer
			clear_cmd(cmd_history[cmd_index - 1]);	  // Clear previous command shown on terminal
			uart_puts(cli_buffer);					  // Show command
			index = strlen(cli_buffer);				  // Update index
		}
	}

	// Tab key is pressed
	else if (input == '\t')
	{
		int found_index = 0; // Indexing the found command
		for (int i = 0; i < 6; i++)
			if (strsearch(commands[i], cli_buffer))
			{
				found_index = i;
				break;
			}

		clear_cmd(cli_buffer);					 // Clear previous command shown on terminal
		feed(commands[found_index], cli_buffer); // Load found command into current buffer
		uart_puts(cli_buffer);					 // Show buffer
		index = strlen(cli_buffer);				 // Update index
	}

	// Put into a buffer until got new line character
	else if (input != '\n')
	{
		// Check for backspace, if not, continue bufferring
		if (input != '\b')
		{
			uart_sendc(input);
			cli_buffer[index] = input; // Store into the buffer
			index++;
		}

		// If backspaced, clear in buffer + clear on terminal
		else if (input == '\b')
		{
			index--;
			if (index >= 0)
			{
				cli_buffer[index] = '\0';	 // Clear 1 char in buffer
				uart_puts("\033[1D\033[0K"); // Clear 1 char in terminal (Move cursor left 1 step & Clear line from cursor left)
			}

			// Restart index if buffer is empty
			else if (index < 0)
				index = 0;
		}
	}

	// Return key is pressed
	else if (input == '\n')
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
			cmd_index = cmd_history_length;			  // Continue indexing
			feed(cli_buffer, cmd_history[cmd_index]); // Save current buffer
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

			if (temp[8] == ' ') // Check for space which means receive another parameter
			{
				temp[8] = '\0'; // Enclose the string
				if (strcmp(temp, commands[2]))
				{
					color_flag = 1;	  // Color flag on to bypass default prompt color
					cmd_option = 's'; // Turn on 'setcolor' with parameter flag
				}
			}
			// Clear temp
			temp[0] = '\0';

			// Check buffer with available commands
			if (strcmp(cli_buffer, commands[0]) || cmd_option == 'h') // help command
			{
				show_help(cli_buffer, cmd_option);
				cmd_option = 'x'; // Reset option flag
			}
			else if (strcmp(cli_buffer, commands[1])) // clear command
				clear_screen();

			else if (strcmp(cli_buffer, commands[2]) || cmd_option == 's') // setcolor command
			{
				set_color(cli_buffer, cmd_option);
				cmd_option = 'x'; // Reset option flag
			}
			else if (strcmp(cli_buffer, commands[3])) // showinfo command
				show_info();

			else if (strcmp(cli_buffer, commands[4])) // about command
				show_about();

			else if (strcmp(cli_buffer, commands[5])) // test command
			{
				// uart_puts("\nChoose test case: ");
				// char value = uart_getc(); // Read each char
				// uart_sendc(value);

				// // Test case for printf function
				// if (value == '1')
				// 	test(value);
				// // Test case for mailbox setup function
				// else if (value == '2')
				// 	test((uint64_t)mBuf);
				// else
				// 	uart_puts("\nInvalid test case. Please try again.\n");

				test('1');
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
		}
	}
}

// Show welcome screen when OS boot up
void show_welcome_screen()
{
	show_about(); // Welcome screen
	uart_puts("\n\tType 'help' to show list of available commands\n");
	uart_puts("\n");
	show_prompt(0);
}

// Show prompt at the beginning of each command
void show_prompt(int flag)
{
	if (flag == 0)
	{
		uart_puts("\x1b[0m");				 // Return to default
		uart_puts("\x1b[1;34mMoonveil\n> "); // Bold & Blue foreground text
		uart_puts("\x1b[0m");				 // Return to default
	}
	else
		uart_puts("Moonveil\n> ");
}

// Output an error message
void show_error(char *cmd, char *error_message)
{
	uart_puts("\nError: '");
	uart_puts(cmd);
	uart_puts("' ");
	uart_puts(error_message);
	uart_puts("\n");
}

// Clear a command on terminal
void clear_cmd(char *command)
{
	for (int i = 0; i < strlen(command); i++)
		uart_puts("\033[1D"); // Cursor to the left n times equal to the length of the buffer
	uart_puts("\033[0K");	  // Clear line from cursor right
}

// Feeding into buffer
void feed(char *command, char *buffer)
{
	for (int i = 0; i < strlen(command); i++)
	{
		buffer[i] = command[i];
		buffer[i + 1] = '\0';
	}
}
