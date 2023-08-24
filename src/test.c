#include "test.h"

void test(int test_case)
{

    if (test_case == '1')
    {
        // int d2 = 16995;
        // uart_puts("\n***** \%d format\n");
        // printf("Value: %d ----> Output: %d\n", d2, d2);

        // int d1 = -42069;
        // printf("\n***** negetive decimal format\n");
        // printf("Value: %d ----> Output: %d\n", d1, d1);

        // int d3 = 821211;
        // printf("\n***** width = 5\n");
        // printf("Value: %d ----> Output: %50d\n", d3, d3);

        int d4 = 11111;
        uart_puts("\n***** 0 flag (\%03d)\n");
        printf("%03d\n", d4, d4);

        // Value: % d-- -- > Output:

        // int h1 = 51966;
        // int h2 = -16;
        // printf("\n***** hexa format\n");
        // printf("Value: %d ----> Output: 0x%x\n", h1, h1);
        // printf("Value: %d ----> Output: 0x%x\n", h2, h2);

        // char ch = 'G';
        // printf("\n***** char format\n");
        // printf("%c\n", ch);

        // char *string = "MOONVEIL";
        // printf("\n***** string format\n");
        // printf("%s\n", string);

        // float f = 0.123456;
        // printf("\n***** float format\n");
        // printf("%f\n", f);

        // uart_puts("\n***** \%\% format\n");
        // printf("%%\n");
    }

    else // This is for mailbox test case, which is test_case == mBuf
    {
        uart_puts("\n***** Test case 7: Mailbox setup\n");

        uint32_t *data = 0;

        printf("\n-- TAG 1: Get board revision");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETREVISION, &data, 0, 4);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Board revision = 0x%x\n", data[0]);

        printf("\n-- TAG 2: Get board MAC address");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETMACADDR, &data, 0, 6);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        uart_puts("\n\t>>> DATA: Board MAC address: ");
        uart_macaddr(data[1], 12);
        uart_sendc(':');
        uart_macaddr(data[0], 28);
        printf("\n");

        printf("\n-- TAG 3: Get clock rate");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETCLKRATE, &data, 4, 8, 3);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Clock ID = %d", data[0]);
        printf("\n\t>>> DATA: ARM clock rate = %d\n", data[1]);

        printf("\n-- TAG 4: Get physical width/height");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETPHYWH, &data, 0, 8);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Width (in px) = %d", data[0]);
        printf("\n\t>>> DATA: Height (in px) = %d\n", data[1]);

        printf("\n-- TAG 5: Set physical width/height");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_SETPHYWH, &data, 8, 8, 1024, 768);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Actual Physical Width = %d", data[0]);
        printf("\n\t>>> DATA: Actual Physical Height = %d\n", data[1]);

        printf("\n-- TAG 6: Get ARM memory");
        mbox_buffer_setup(ADDR(test_case), MBOX_TAG_GETARMMEMORY, &data, 0, 8);
        mbox_call(ADDR(test_case), MBOX_CH_PROP);
        printf("\n\t>>> DATA: Base address in byte = %d", data[0]);
        printf("\n\t>>> DATA: Size in byte = %x\n", data[1]);
    }
}

void test_info()
{
    uart_puts("\nTesting two new features of the OS: ");
    uart_puts("\n\t1. printf function: implement the standard printf of C language");
    uart_puts("\n\t2. mailbox setup: make our code for mailbox setup look cleaner and increase its ease of use\n");

    uart_puts("\n\tEnter test case number to proceed.\n");
}
