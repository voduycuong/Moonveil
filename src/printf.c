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
	int zero_flag = 0;
	int zero_num = 0;

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

			// Check if 0 flag is on and check for the next character
			if (*string == '0')
			{
				zero_flag = 1;
				string++;
			}

			if (*string >= '1' && *string <= '9')
			{
				zero_num = zero_num * 10 + (*string - '0');
				string++;
			}

			// if (zero_num == 0)
			// 	zero_flag = 0;

			// uart_puts("\n");
			// uart_dec(zero_num);
			// uart_puts("\n");
			// uart_dec(zero_flag);
			// uart_puts("\n");

			if (*string == 'd')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				if (x < 0)
				{
					buffer[buffer_index] = '-';
					buffer_index++;
					x *= -1;
				}
				do
				{
					temp_buffer[temp_index] = (x % 10) + '0';
					temp_index--;
					x /= 10;
				} while (x != 0);

				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
				{
					if (zero_flag && zero_num < (MAX_PRINT_SIZE - temp_index))
					{
						for (buffer_index = 0; MAX_PRINT_SIZE - temp_index - zero_num; buffer_index++)
							buffer[buffer_index] = '0';
					}
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
				long int x = va_arg(ap, long int);
				int temp_index = MAX_PRINT_SIZE - 1;
				static char hex_char[] = "0123456789ABCDEF";

				if (x < 0)
					x = 4294967295 + x + 1;
				/*
				 * The result of negetive will be shown as two's complement
				 * Step 1: take complement of equivalent positive x -----> 4294967295 - (-x) = 4294967295 + x
				 * 4294967295 = 2^32 (4 byte) in decimal
				 * Step 2: adding 1 to the complement number, ignoring any overflow.
				 */

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
