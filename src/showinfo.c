#include "showinfo.h"

// showinfo command
void show_info()
{
    volatile uint32_t __attribute__((aligned(16))) old_mBuf[36];

    old_mBuf[0] = 12 * 4;       // Message Buffer Size in bytes (11 elements * 4 bytes (32 bit) each)
    old_mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)

    old_mBuf[2] = 0x00010002; // TAG Identifier: Get board revision
    old_mBuf[3] = 4;          // Value buffer size in bytes (max of request and response lengths)
    old_mBuf[4] = 0;          // REQUEST CODE = 0
    old_mBuf[5] = 0;          // clear output buffer (response data are old_mBuf[5])

    old_mBuf[6] = 0x00010003; // TAG Identifier: Get board MAC address
    old_mBuf[7] = 6;          // Value buffer size in bytes (max of request and response lengths)
    old_mBuf[8] = 0;          // REQUEST CODE = 0
    old_mBuf[9] = 0;          // clear output buffer (response data are old_mBuf[9])
    old_mBuf[10] = 0;         // clear output buffer (response data are old_mBuf[10])

    old_mBuf[11] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(old_mBuf), MBOX_CH_PROP))
    {
        uart_puts("Response Code for whole message: ");
        uart_hex(old_mBuf[1]);
        uart_puts("\n");

        uart_puts("Response Code in Message TAG: ");
        uart_hex(old_mBuf[4]);
        uart_puts("\n");

        uart_puts("----------------------------------------------\n");

        uart_puts("Board model: \n\t");
        uart_hex(old_mBuf[5]);
        uart_puts("\n");

        uart_puts("Board MAC address: \n\t");
        uart_macaddr(old_mBuf[10], 12);
        uart_sendc('-');
        uart_macaddr(old_mBuf[9], 28);
        uart_puts("\n");
    }
    else
    {
        uart_puts("Unable to query!\n");
    }
}

// showinfo command infomation
void show_info_info()
{
    uart_puts("\n");
    uart_puts("Show board revision and board MAC address.\n");
}