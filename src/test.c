#include "test.h"

void test(char *test_case)
{
    if (strcmp(test_case, "test_printf"))
    {
        uart_puts("\n-- Test case 1: \%d format\n");
        printf("%d", 100000);
        uart_puts("\n");

        uart_puts("\n-- Test case 2: \%x format\n");
        printf("%x", 0xFAFB);
        uart_puts("\n");

        uart_puts("\n-- Test case 3: \%c format\n");
        printf("%c", 'C');
        uart_puts("\n");

        uart_puts("\n-- Test case 4: \%s format\n");
        printf("%s", "MOONVEIL");
        uart_puts("\n");

        uart_puts("\n-- Test case 5: \%f format\n");
        printf("%f", 0.123456);
        uart_puts("\n");

        uart_puts("\n-- Test case 6: \%\% format\n");
        printf("%%");
        uart_puts("\n");
    }

    else if (strcmp(test_case, "test_mailbox"))
    {
        uart_puts("\n-- Test case 1: Get board revison");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETREVISION);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("\nDATA: Board revision = ");
        uart_hex(mBuf[5]);
        uart_puts("\n");

        uart_puts("\n-- Test case 2: Get board revison");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETMACADDR);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("DATA: Board MAC address: \n\t");
        uart_macaddr(mBuf[6], 12);
        uart_sendc(':');
        uart_macaddr(mBuf[5], 28);
        uart_puts("\n");

        uart_puts("\n-- Test case 3: Get clock rate");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETCLKRATE, 3);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("\nDATA: Clock ID =  ");
        uart_dec(mBuf[5]);
        uart_puts("\nDATA: ARM clock rate = ");
        uart_dec(mBuf[6]);
        uart_puts("\n");

        uart_puts("\n-- Test case 4: Set physical width/height");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, 1024, 768);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("\nDATA: Got Actual Physical Width: ");
        uart_dec(mBuf[5]);
        uart_puts("\nDATA: Got Actual Physical Height: ");
        uart_dec(mBuf[6]);
        uart_puts("\n");
    }
}

void test_info()
{
    uart_puts("\n");
    uart_puts("Testing two new features of the program: ");
    uart_puts("\n\t1. printf command: implement the standard printf of C programming");
    uart_puts("\n\t2. mailbox setup: make our code for mailbox setup look cleaner and increase its ease of use");
    uart_puts("\n");
}
