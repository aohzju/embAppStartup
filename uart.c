/*
 * uart.c
 *
 */

#include "uart.h"
#include <stdint.h>

//static int uartdr_offset = 0;
static int uartctl_offset = 0x0030;

void enable_uart(UART_PL011 uart)
{
	short *ctrl = (short*)((int64_t)uart + uartctl_offset);

	*ctrl = 0x0B01;
}

void write_uart_buf(UART_PL011 uart, const char *buf, int n)
{
	int i;
	char *uartdr = (char*)uart;
	for(i=0; i<n; i++)
		*uartdr = buf[i];
}

void write_uart_string(UART_PL011 uart, const char *str)
{
	char ch;
	char *uartdr = (char*)uart;
	while(1){
		ch = *str++;
		if(ch)
			*uartdr = ch;
		else
			break;
	}
}

void write_uart_char(UART_PL011 uart, char ch)
{
	char *uartdr = (char*)uart;
	*uartdr = ch;
}
