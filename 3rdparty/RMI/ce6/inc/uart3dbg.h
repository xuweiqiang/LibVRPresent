// Copyright (c) 2001-2004, BSQUARE Corporation.  All rights reserved.
//
// low-level serial port routines for debugging.
//

typedef struct _serial_device {
    unsigned __int32 rxdata;		// 0x00
    unsigned __int32 txdata;		// 0x04
    unsigned __int32 inten;		// 0x08
    unsigned __int32 intcause;		// 0x0c
    unsigned __int32 fifoctrl;		// 0x10
    unsigned __int32 linectrl;		// 0x14
    unsigned __int32 mdmctrl;		// 0x18
    unsigned __int32 linestat;		// 0x1c
    unsigned __int32 mdmstat;		// 0x20
             __int32 unused;		// 0x24
    unsigned __int32 clkdiv;		// 0x28
             __int32 unused2[(0x100 - 0x2c) / sizeof (__int32)];	// 0x2c
    unsigned __int32 enable;		// 0x100
} serial_device_t;

#define	UART_TE		(1U << 6)
#define	UART_DR		(1U << 0)

#define	UART0_BASE	0x11100000
#define	UART1_BASE	0x11200000
#define	UART2_BASE	0x11300000
#define	UART3_BASE	0x11400000

#define	KSEG1_ADDRESS	0xA0000000

#define uart0	(*(serial_device_t *)(UART0_BASE | KSEG1_ADDRESS))
#define uart1	(*(serial_device_t *)(UART0_BASE | KSEG1_ADDRESS))
#define uart2	(*(serial_device_t *)(UART0_BASE | KSEG1_ADDRESS))
#define uart3	(*(serial_device_t *)(UART0_BASE | KSEG1_ADDRESS))

static void
debug_putchar(int c)
{
    do {} while ((uart0.linestat & UART_TE) == 0);
    uart0.txdata = (unsigned __int32)(c & 0xff);
}

static void
debug_puts(unsigned short *s)
{
    while (*s != '\0')
        debug_putchar(*s++);
}

static void
debug_puthex(unsigned __int32 value)
{
    int i;

    for (i = 28; i >= 0; i -= 4)
        debug_putchar("0123456789ABCDEF"[(value >> i) & 0x0f]);
}

#if (0)
static void
debug_printf(short *fmt, ...)
{
    unsigned __int32 *args = (void *)&fmt;

    while (*fmt != '\0')
    {
        if (*fmt == '%')
        {
            ++fmt;
            if (*fmt == '\0')
                break;
            if (*fmt != 'x' && *fmt != 'X')
            {
                debug_putchar('?');
                debug_putchar(*fmt++);
                continue;
            }
            ++fmt;
            debug_puthex(*++args);
            continue;
        }
        debug_putchar(*fmt++);
    }    
}
#endif

