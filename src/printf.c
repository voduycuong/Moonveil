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
			break;

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
				int x = va_arg(ap, int); // Retrieve next argument
				uart_sendc(x);
			}

			else if (*string == 's')
			{
				int x = va_arg(ap, int); // Retrieve next argument
				uart_puts(x);
			}

			else if (*string == 'f')
			{
				// string++;
				// int x = va_arg(ap, int); // Retrieve next argument
				// uart_puts(x);
				// int temp_index = MAX_PRINT_SIZE - 1;
			}

			else if (*string == 'x')
				y
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
					uart_puts("0x");
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

	// Print out formated string
	uart_puts(buffer);
}
