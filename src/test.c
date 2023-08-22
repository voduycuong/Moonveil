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

        uart_puts("\n-- Test case 6: \%% \n");
        printf("%%\n");
    }

    else if (strcmp(test_case, "test_mailbox"))
    {
        uint32_t *physize = 0;
        unsigned int *buffer_ptr = (unsigned int *)((unsigned long)buffer_addr);

        printf("\n-- Test case 1: Get board revision");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETREVISION, &physize, 0, 4);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\n>>> DATA: Board revision = 0x%x", physize[0]);
        printf("\n");

        printf("\n-- Test case 2: Get board MAC address");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETMACADDR, &physize, 0, 6);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        uart_puts("\n>>> DATA: Board MAC address: ");
        uart_macaddr(mBuf[6], 12);
        uart_sendc(':');
        uart_macaddr(mBuf[5], 28);
        uart_puts("\n");
        printf("\n");

        printf("\n-- Test case 3: Get clock rate");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETCLKRATE, &physize, 4, 8, 3);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\n>>> DATA: Clock ID = %d", physize[0]);
        printf("\n>>> DATA: ARM clock rate = %d", physize[1]);
        printf("\n");

        printf("\n-- Test case 4: Get physical width/height");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETPHYWH, &physize, 0, 8);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\n>>> DATA: Width (in px) = %d", physize[0]);
        printf("\n>>> DATA: Height (in px) = %d", physize[1]);
        printf("\n");

        printf("\n-- Test case 5: Set physical width/height");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, &physize, 8, 8, 1024, 768);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);

        printf("\n>>> DATA: Actual Physical Width = %d", physize[0]);
        printf("\n>>> DATA: Actual Physical Height = %d", physize[1]);
        printf("\n");

        printf("\n-- Test case 6: Get ARM memory");
        mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETARMMEMORY, &physize, 0, 8);
        mbox_call(ADDR(mBuf), MBOX_CH_PROP);
        printf("\n>>> DATA: Base address in byte = %d", physize[0]);
        printf("\n>>> DATA: Size in byte = %x", physize[1]);
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
