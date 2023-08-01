// -----------------------------------uart0.c -------------------------------------
#include "uart.h"
#include "gpio.h"

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void uart_init()
{
	register unsigned int r;

	/* Turn off UART0 */
	UART0_CR = 0x0;

	/* Setup GPIO pins 32 and 33 */

	/* Set GPIO32 and GPIO33 to be PL011 TX/RX which is ALT3 */
	r = GPFSEL3;
	r &= ~((7 << 6) | (7 << 9)); // Clear bits 11-6 (GPIO32, GPIO33)
	r |= (7 << 6) | (7 << 9);	 // Set value 7 (0b111 - ALT3: UART0)
	GPFSEL3 = r;

#ifdef RPI3 // RBP3
	/* enable GPIO 32, 33 */
	GPPUD = 0; // No pull up/down control
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}								 // waiting 150 cycles
	GPPUDCLK1 = (1 << 0) | (1 << 1); // enable clock for GPIO 32, 33
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}			   // waiting 150 cycles
	GPPUDCLK1 = 0; // flush GPIO setup

#else // RPB4
	r = GPIO_PUP_PDN_CNTRL_REG2;
	r &= ~((3 << 0) | (3 << 2)); // Clear Resistor Select for GPIO 14, 15
	GPIO_PUP_PDN_CNTRL_REG2 = r;
#endif

	/* Mask all interrupts. */
	UART0_IMSC = 0;

	/* Clear pending interrupts. */
	UART0_ICR = 0x7FF;

	/* Set integer & fractional part of Baud rate
	Divider = UART_CLOCK/(16 * Baud)
			= 48 * 10^6/ (16 * 38400) = 78.125
	Default UART_CLOCK = 48MHz (old firmware it was 3MHz);
	Integer part register UART0_IBRD  = integer part of Divider
	Fraction part register UART0_FBRD = (Fractional part * 64) + 0.5
									  = (0.125 * 64) + 0.5
									  = 8.5 */

	// 38400 baud
	UART0_IBRD = 78;
	UART0_FBRD = 9;

	/* Set up the Line Control Register */
	/* Enable FIFO */
	/* Set length to 8 bit */
	/* Defaults for other bit are No parity, 1 stop bit */
	UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT;

	/* Enable UART0, receive, and transmit */
	UART0_CR = 0x301; // enable Tx, Rx, FIFO
}

/**
 * Send a character
 */
void uart_sendc(char c)
{
	/* Check Flags Register */
	/* And wait until transmitter is not full */
	do
	{
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_TXFF);

	/* Write our data byte out to the data register */
	UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc()
{
	char c = 0;
	/* Check Flags Register */
	/* Wait until Receiver is not empty
	 * (at least one byte data in receive fifo)*/
	do
	{
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_RXFE);

	/* read it and return */
	c = (unsigned char)(UART0_DR);

	/* convert carriage return to newline */
	return (c == '\r' ? '\n' : c);
}

/**
 * Display a string
 */
void uart_puts(char *s)
{
	while (*s)
	{
		/* convert newline to carriage return + newline */
		if (*s == '\n')
			uart_sendc('\r');
		uart_sendc(*s++);
	}
}

/**
 * Display a binary value in hexadecimal
 */
void uart_hex(unsigned int d)
{
	unsigned int n;
	int c;
	uart_puts("0x");
	for (c = 28; c >= 0; c = c - 4)
	{
		// Get highest 4-bit nibble
		n = (d >> c) & 0xF;
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		n += (n > 9) ? (-10 + 'A') : '0';
		uart_sendc(n);
	}
}

/**
 * Display a value in decimal by
 */
void uart_dec(int num)
{
	char str[33] = "";
	int i, rem, len = 0, n;
	n = num;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	if (num == 0)
		len = 1;
	for (i = 0; i < len; i++)
	{
		rem = num % 10;
		num = num / 10;
		str[len - (i + 1)] = rem + '0';
	}
	str[len] = '\0';
	uart_puts(str);