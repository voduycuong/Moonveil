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

	int16_t width = 0;
	int16_t zero_flag = 0;
	int16_t width_flag = 0;

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

			// Calculate both 0 flag and width
			while (*string >= '0' && *string <= '9')
			{
				width = width * 10 + (*string - '0');
				string++;
			}
			if (width > 0)
				width_flag = 1;

			if (*string == 'd')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				// Check for negative number
				if (x < 0)
				{
					buffer[buffer_index++] = '-'; // Put negative sign
					x *= -1;					  // Invert number
				}
				do
				{
					temp_buffer[temp_index--] = (x % 10) + '0';
					x /= 10;
				} while (x != 0);

				// Check for 0 flag or width
				do
				{
					if (zero_flag)
						temp_buffer[temp_index--] = '0';
					else if (width_flag)
						temp_buffer[temp_index--] = ' ';
				} while (MAX_PRINT_SIZE - temp_index <= width);

				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
					buffer[buffer_index++] = temp_buffer[i];
			}

			else if (*string == 'c')
			{
				string++;
				int x = va_arg(ap, int); // Retrieve next argument
				buffer[buffer_index++] = x;
			}

			else if (*string == 's')
			{
				string++;
				char *x = va_arg(ap, char *); // Retrieve next argument
				for (int i = 0; i < strlen(x); i++)
					buffer[buffer_index++] = x[i];
			}

			else if (*string == 'f')
			{
				string++;
				double x = va_arg(ap, double); // Retrieve next argument
				int temp_index = MAX_PRINT_SIZE - 1;

				double n = (x - (int)x); // Get the fractional part
				while (n > 0)			 // Loop until x is zero
				{
					n *= 10;								  // Multiply x by 10
					temp_buffer[temp_index--] = '0' + (int)n; // Store the first digit
					n -= (int)n;							  // Remove the first digit
				}

				temp_buffer[temp_index--] = '.'; // Add decimal point

				int m = (int)x; // Get the integer part
				while (m > 0)	// Loop until m is zero
				{
					temp_buffer[temp_index--] = '0' + m % 10; // Store the last digit
					m /= 10;								  // Remove the last digit
				}

				// Feed temp_buffer to buffer from left to right
				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
				{
					buffer[buffer_index++] = temp_buffer[i];
					if (temp_buffer[i] == '.')
						break;
				}

				// Feed temp_buffer to buffer from right to left
				for (int i = MAX_PRINT_SIZE - 1; i > temp_index; i--)
				{
					if (temp_buffer[i] == '.')
						break;
					buffer[buffer_index++] = temp_buffer[i];
				}
			}

			else if (*string == 'x')
			{
				string++;
				long int x = va_arg(ap, long int); // Retrieve next argument
												   // long int data type for two's complement output
				int temp_index = MAX_PRINT_SIZE - 1;
				static char hex_char[] = "0123456789ABCDEF";

				if (x < 0)
					x = 4294967295 + x + 1; // Convert to two's complement

				// Convert to hex
				do
				{
					temp_buffer[temp_index--] = hex_char[x % 16];
					x /= 16;
				} while (x != 0);

				// Check for 0 flag or width
				do
				{
					if (zero_flag)
						temp_buffer[temp_index--] = '0';
					else if (width_flag)
						temp_buffer[temp_index--] = ' ';
				} while (MAX_PRINT_SIZE - temp_index <= width);

				// Print out hex format indicator
				buffer[buffer_index++] = '0';
				buffer[buffer_index++] = 'x';

				// Feed temp_ buffer to buffer
				for (int i = temp_index + 1; i < MAX_PRINT_SIZE; i++)
					buffer[buffer_index++] = temp_buffer[i];
			}

			else if (*string == '%')
				uart_sendc('%');
		}

		else
		{
			buffer[buffer_index++] = *string;
			string++;
		}

		if (buffer_index == MAX_PRINT_SIZE - 1)
			break;
	}

	va_end(ap); // End using variable argument list

	// Print out the buffer
	uart_puts(buffer);
}