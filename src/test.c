#include "test.h"

void test(char *test_case)
{
    if (strcmp(test_case, "test_printf"))
    {
        int d = 100000;
        printf("\n-- Test case 1: decimal format\n");
        printf("%d\n", d);

        int h = 0xFAFB;
        printf("\n-- Test case 2: hexa format\n");
        printf("0x%x\n", h);

        char ch = 'G';
        printf("\n-- Test case 3: char format\n");
        printf("%c\n", ch);

        char *string = "MOONVEIL";
        printf("\n-- Test case 4: string format\n");
        printf("%s\n", string);

        float f = 0.123456;
        printf("\n-- Test case 5: float format\n");
        printf("%f\n", f);

        printf("\n-- Test case 6: %% format\n");
        printf("%%\n");
    }

    else if (strcmp(test_case, "test_mailbox"))
    {
        printf("\n-- Test case 1: Get board revision");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETREVISION, 4);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\nDATA: Board revision = 0x%x", mBuf[5]);
        printf("\n");

        printf("\n-- Test case 2: Get board MAC address");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETMACADDR, 6);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("\nDATA: Board MAC address: ");
        uart_macaddr(mBuf[6], 12);
        uart_sendc(':');
        uart_macaddr(mBuf[5], 28);
        uart_puts("\n");
        printf("\n");

        printf("\n-- Test case 3: Get clock rate");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETCLKRATE, 8, 3);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\nDATA: Clock ID = %d", mBuf[5]);
        printf("\nDATA: ARM clock rate = %d", mBuf[6]);
        printf("\n");

        printf("\n-- Test case 4: Get physical width/height");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETPHYWH, 8);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\nDATA: Width (in px) = %d", mBuf[5]);
        printf("\nDATA: Height (in px) = %d", mBuf[6]);
        printf("\n");

        printf("\n-- Test case 5: Set physical width/height");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, 8, 1024, 768);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\nDATA: Actual Physical Width = %d", mBuf[5]);
        printf("\nDATA: Actual Physical Height = %d", mBuf[6]);
        printf("\n");

        printf("\n-- Test case 6: Get voltage");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETVOLT, 8, 1);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\nDATA: Volt ID = %d", mBuf[5]);
        printf("\nDATA: Voltage = %x", mBuf[6]);
        printf("\n");
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
