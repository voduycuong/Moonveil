#include "test.h"

void test(char *test_case)
{
    if (test_case == "test_printf")
    {
        int test_hex = 0xFAFB;
        uart_puts("\n");
        printf("%x\t%d", test_hex, test_hex);

        char test_c = 'C';
        uart_puts("\n");
        printf("%c", test_c);

        char *test_string = "Cuong";
        uart_puts("\n");
        printf("%s", test_string);

        float test_float = 0.1234;
        uart_puts("\n");
        printf("%f", test_float);
    }

    else if (test_case == "test_mailbox")
    {
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, 1024, 768);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("\nGot Actual Physical Width: ");
        uart_dec(mBuf[5]);
        uart_puts("\nGot Actual Physical Height: ");
        uart_dec(mBuf[6]);

        // mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETMACADDR, 0);
        // mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        // uart_puts("\nMAC 1: ");
        // uart_hex(mBuf[5]);
        // uart_puts("\nMAC 2: ");
        // uart_hex(mBuf[6]);

        // mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETCLKRATE, 3);
        // mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        // uart_puts("\nDATA: Clock ID =  ");
        // uart_dec(mBuf[5]);
        // uart_puts("\nDATA: ARM clock rate = ");
        // uart_dec(mBuf[6]);

        // mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETREVISION);
        // mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        // uart_puts("\nBoard revision = ");
        // uart_hex(mBuf[5]);

        // uart_puts("\n");

        // mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETMACADDR);
        // mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        // uart_puts("Board MAC address: \n\t");
        // uart_macaddr(mBuf[6], 12);
        // uart_sendc(':');
        // uart_macaddr(mBuf[5], 28);
        // uart_puts("\n");
    }
}