#include "test.h"

void test(int test_case)
{

    if (test_case == '1')
    {
        uart_puts("\n***** Test case 1: printf() function\n");

        char delay = uart_getc(); // Pseudo-delay for demo

        uart_puts("\n***** \%d format\n");
        int d1 = 16995;
        int d2 = -42069;
        printf("Value: %d ----> Output (normal): %d\n", d1, d1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (precision = 10): %.10d\n", d1, d1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (precision = 2): %.2d\n", d1, d1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (width = 10): %10d\n", d1, d1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (width = 2): %2d\n", d1, d1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output: %d\n", d2, d2);

        delay = uart_getc(); // Press any key to proceed

        uart_puts("\n***** \%x format\n");
        int h1 = 51966;
        int h2 = -16;
        printf("Value: %d ----> Output (normal): %x\n", h1, h1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (precision = 10): %.10x\n", h1, h1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (precision = 2): %.2x\n", h1, h1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (width = 10): %10x\n", h1, h1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output (width = 2): %2x\n", h1, h1);
        delay = uart_getc(); // Pseudo-delay for demo
        printf("Value: %d ----> Output: %x\n", h2, h2);

        delay = uart_getc(); // Press any key to proceed

        uart_puts("\n***** \%c format\n");
        char ch = 'G';
        printf("%c\n", ch);

        delay = uart_getc(); // Press any key to proceed

        uart_puts("\n***** \%s format\n");
        char *string = "MOONVEIL";
        printf("%s\n", string);

        delay = uart_getc(); // Press any key to proceed

        uart_puts("\n***** \%f format\n");
        double f1 = 10.1244444;
        double f2 = -10.1244444;
        printf("Positive number: %f\nNegative number: %f\n", f1, f2);
        delay = uart_getc(); // Press any key to proceed
        printf("Precision = 3 -> %.3f\nWidth = 10 & Precision = 3 -> %10.3f\n", f1, f1);

        delay = uart_getc(); // Press any key to proceed

        uart_puts("\n***** \%\% format\n");
        printf("%%\n");

        uart_sendc(delay - delay); // To avoid warning 'variable not used'
    }

    else // This is for mailbox test case, which is test_case == mBuf
    {
        uart_puts("\n***** Test case 2: Mailbox setup\n");

        uint32_t *data = 0;

        char delay = uart_getc(); // Pseudo-delay for demo

        printf("\n-- TAG 1: Get board revision");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETREVISION, &data, 0, 4);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Board revision = 0x%x\n", data[0]);

        delay = uart_getc(); // Press any key to proceed

        printf("\n-- TAG 2: Get board MAC address");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETMACADDR, &data, 0, 6);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        uart_puts("\n\t>>> DATA: Board MAC address: ");
        uart_macaddr(data[1], 12);
        uart_sendc('-');
        uart_macaddr(data[0], 28);
        printf("\n");

        delay = uart_getc(); // Press any key to proceed

        printf("\n-- TAG 3: Get clock rate");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETCLKRATE, &data, 4, 8, 3);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Clock ID = %d", data[0]);
        printf("\n\t>>> DATA: ARM clock rate = %d\n", data[1]);

        delay = uart_getc(); // Press any key to proceed

        printf("\n-- TAG 4: Set clock rate");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_SETCLKRATE, &data, 12, 8, 2, 10000000, 1);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Clock ID = %d", data[0]);
        printf("\n\t>>> DATA: UART clock rate = %d\n", data[1]);

        delay = uart_getc(); // Press any key to proceed

        printf("\n-- TAG 5: Get physical width/height");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETPHYWH, &data, 0, 8);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Width (in px) = %d", data[0]);
        printf("\n\t>>> DATA: Height (in px) = %d\n", data[1]);

        delay = uart_getc(); // Press any key to proceed

        printf("\n-- TAG 6: Set physical width/height");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_SETPHYWH, &data, 8, 8, 1024, 768);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Actual Physical Width = %d", data[0]);
        printf("\n\t>>> DATA: Actual Physical Height = %d\n", data[1]);

        uart_sendc(delay - delay); // To avoid warning 'variable not used'
    }
}

void test_info()
{
    uart_puts("\nTesting two new features of the OS: ");
    uart_puts("\n\t1. printf function: implement the standard printf of C language");
    uart_puts("\n\t2. mailbox setup: make our code for mailbox setup look cleaner and increase its ease of use\n");

    uart_puts("\n\tEnter test case number to proceed.\n");
}
