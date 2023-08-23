#include "test.h"

void test(int32_t mBuf)
{
    uint32_t *data = 0;

    printf("\n-- Test case 1: Get board revision");
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETREVISION, &data, 0, 4);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    printf("\n>>> DATA: Board revision = 0x%x", data[0]);
    printf("\n");

    printf("\n-- Test case 2: Get board MAC address");
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETMACADDR, &data, 0, 6);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    uart_puts("\n>>> DATA: Board MAC address: ");
    uart_macaddr(data[1], 12);
    uart_sendc(':');
    uart_macaddr(data[0], 28);
    printf("\n");

    printf("\n-- Test case 3: Get clock rate");
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETCLKRATE, &data, 4, 8, 3);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    printf("\n>>> DATA: Clock ID = %d", data[0]);
    printf("\n>>> DATA: ARM clock rate = %d", data[1]);
    printf("\n");

    printf("\n-- Test case 4: Get physical width/height");
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETPHYWH, &data, 0, 8);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    printf("\n>>> DATA: Width (in px) = %d", data[0]);
    printf("\n>>> DATA: Height (in px) = %d", data[1]);
    printf("\n");

    printf("\n-- Test case 5: Set physical width/height");
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, &data, 8, 8, 1024, 768);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);

    printf("\n>>> DATA: Actual Physical Width = %d", data[0]);
    printf("\n>>> DATA: Actual Physical Height = %d", data[1]);
    printf("\n");

    printf("\n-- Test case 6: Get ARM memory");
    mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_GETARMMEMORY, &data, 0, 8);
    mbox_call(ADDR(mBuf), MBOX_CH_PROP);
    printf("\n>>> DATA: Base address in byte = %d", data[0]);
    printf("\n>>> DATA: Size in byte = %x", data[1]);
    printf("\n");
}

void test_info()
{
    uart_puts("\n");
    uart_puts("Testing two new features of the program: ");
    uart_puts("\n\t1. printf command: implement the standard printf of C programming");
    uart_puts("\n\t2. mailbox setup: make our code for mailbox setup look cleaner and increase its ease of use");
    uart_puts("\n");
}
