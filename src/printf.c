#include "printf.h"

#define MAX_PRINT_SIZE 256

/*
 * va_arg returns the current argument. va_copy, va_start and va_end don't return values.
 */

void printf(char *string, ...)
{
	va_list ap;			  // Type to hold information about variable arguments (type)
	va_start(ap, string); // Initialize a variable argument list (macro)

	char buffer[MAX_PRINT_SIZE];
	int buffer_index = 0;

	char temp_buffer[MAX_PRINT_SIZE];

	while (1)
	{
		if (*string == 0)
		{
			buffer[buffer_index] = '\0'; // Enclose the buffer
			break;
		}

		if (*string == '%')
		{
			string++;
			if (*string == 'd')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				do
				{
					temp_buffer[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while (x != 0);

				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
				{
					buffer[buffer_index] = temp_buffer[i];
					buffer_index++;
				}
			}

			else if (*string == 'c')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				buffer[buffer_index] = x;
				buffer_index++;
			}

			else if (*string == 's')
			{
				string++;
				char *x = va_arg(ap, char *); // Retrieve next argument
				for (int i = 0; i < strlen(x); i++)
				{
					buffer[buffer_index] = x[i];
					buffer_index++;
				}
			}

			else if (*string == 'f')
			{
				double x = va_arg(ap, double); // Retrieve next argument
				buffer[buffer_index] = x;
				buffer_index++;
			}

			else if (*string == 'x')
			{
				string++;
				int x = va_arg(ap, int);
				int temp_index = MAX_PRINT_SIZE - 1;
				static char hex_char[] = "0123456789ABCDEF";

				do
				{
					temp_buffer[temp_index] = hex_char[x % 16];
					temp_index--;
					x /= 16;
				} while (x != 0);

				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
				{
					buffer[buffer_index] = temp_buffer[i];
					buffer_index++;
				}
			}

			else if (*string == '%')
			{
				uart_sendc('%');
			}
		}

		else
		{
			buffer[buffer_index] = *string;
			buffer_index++;
			string++;
		}

		if (buffer_index == MAX_PRINT_SIZE - 1)
			break;
	}

	va_end(ap); // End using variable argument list

	// Print out the buffer
	uart_puts(buffer);
}
