// -----------------------------------mbox.c -------------------------------------
#include "mbox.h"

/* Mailbox Data Buffer (each element is 32-bit)*/
/*
 * The keyword attribute allows you to specify special attributes
 *
 * The aligned(N) attribute aligns the current data item on an address
 * which is a multiple of N, by inserting padding bytes before the data item
 *
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
 * *
 * We must ensure that our our buffer is located at a 16 byte aligned address,
 * so only the high 28 bits contain the address
 * (last 4 bits is ZERO due to 16 byte alignment)
 *
 */

volatile uint32_t __attribute__((aligned(16))) mBuf[36];

/**
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (*MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = *MBOX0_READ;
    } while ((res & 0xF) != channel);
    return res;
}

/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
        asm volatile("nop");
    } while (*MBOX1_STATUS & MBOX_FULL);
    // send the message
    *MBOX1_WRITE = msg;
}

/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(uint32_t buffer_addr, unsigned char channel)
{
    // Check Buffer Address
    uart_puts("\nBuffer Address: ");
    uart_hex(buffer_addr);
    uart_sendc('\n');
    // Prepare Data (address of Message Buffer)
    uint32_t msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);
    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            uart_puts("Got successful response \n");
        return (mBuf[1] == MBOX_RESPONSE);
    }
    return 0;
}
/*
 * Make a mailbox setup
 * buffer_addr: address of the being used mailbox buffer
 * tag_identifier: TAG indentifier value
 * res_data: pointer of pointer, used to get the first address of response data
 * ...: list of parameters for request values (if necessary).
 */
void mbox_buffer_setup(uint32_t buffer_addr, uint32_t tag_identifier, ...)
{
    va_list ap;                   // Type to hold information about variable arguments (type)
    va_start(ap, tag_identifier); // Initialize a variable argument list (macro)

    uint32_t i = 0;

    mBuf[i++] = 0;              // mBuf[0]: will be filled later at the end.
    mBuf[i++] = MBOX_REQUEST;   // Message Request Code (this is a request message)
    mBuf[i++] = tag_identifier; // TAG Identifier
    mBuf[i++] = 8;              // Value buffer size in bytes
    mBuf[i++] = 0;              // REQUEST CODE = 0

    while (1)
    {
        uint32_t x = va_arg(ap, int);
        if (x != 0)
            mBuf[i++] = x;

        else
            break;
    }

    mBuf[i++] = MBOX_TAG_LAST;
    mBuf[0] = i * 4; // Message Buffer Size in bytes (4 bytes (32 bit) each)

    va_end(ap); // End using variable argument list
}