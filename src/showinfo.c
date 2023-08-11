#include "showinfo.h"

// showinfo command
void show_info()
{
    mBuf[0] = 12 * 4;       // Message Buffer Size in bytes (11 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)

    mBuf[2] = 0x00010002; // TAG Identifier: Get board revision
    mBuf[3] = 4;          // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0;          // REQUEST CODE = 0
    mBuf[5] = 0;          // clear output buffer (response data are mBuf[5])

    mBuf[6] = 0x00010003; // TAG Identifier: Get board MAC address
    mBuf[7] = 6;          // Value buffer size in bytes (max of request and response lengths)
    mBuf[8] = 0;          // REQUEST CODE = 0
    mBuf[9] = 0;          // clear output buffer (response data are mBuf[9])
    mBuf[10] = 0;         // clear output buffer (response data are mBuf[10])

    mBuf[11] = MBOX_TAG_LAST;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("Response Code for whole message: ");
        uart_hex(mBuf[1]);
        uart_puts("\n");

        uart_puts("Response Code in Message TAG: ");
        uart_hex(mBuf[4]);
        uart_puts("\n");

        uart_puts("-- Board model: \n\t");
        uart_hex(mBuf[5]);
        uart_puts("\n");

        uart_puts("-- Board MAC address: \n\t");

        uart_dec((mBuf[10] & (0xF << 12)) >> 12);
        uart_dec((mBuf[10] & (0xF << 8)) >> 8);
        uart_sendc(':');
        uart_dec((mBuf[10] & (0xF << 4)) >> 4);
        uart_dec(mBuf[10] & 0xF);
        uart_sendc(':');
        uart_dec((mBuf[9] & (0xF << 28)) >> 28);
        uart_dec((mBuf[9] & (0xF << 24)) >> 24);
        uart_sendc(':');
        uart_dec((mBuf[9] & (0xF << 20)) >> 20);
        uart_dec((mBuf[9] & (0xF << 16)) >> 16);
        uart_sendc(':');
        uart_dec((mBuf[9] & (0xF << 12)) >> 12);
        uart_dec((mBuf[9] & (0xF << 8)) >> 8);
        uart_sendc(':');
        uart_dec((mBuf[9] & (0xF << 4)) >> 4);
        uart_dec(mBuf[9] & 0xF);

        uart_puts("\n");
    }
    else
    {
        uart_puts("Unable to query!\n");
    }
}

// showinfo command info
void show_info_info()
{
    uart_puts("\n");
    uart_puts("Show board revision and board MAC address.\n");
}