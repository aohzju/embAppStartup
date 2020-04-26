/*
 ============================================================================
 */
#include "config.h"
#include "uart.h"
#include "basehw.h"

//Globals:
int array1[16] = {3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12}; //RW Data
int array2[32] = {0}; //ZI, will goes to .bss
int array3[64]; //Global or statically allocated variables not explicitly initialized to any value
				//Will goes to .bss by ARM compiler
				//Will goes to *COM* section by gcc if -fno-common is not used
int array4[16] __attribute__((section("configData"))) = { 100, 101, 102, 103}; //A named data section

#define uart0 0x001C090000LL
char *uartdr0 = (char*) uart0;
short *uartctl0 = (short*)(uart0 + 0x0030);

#define GenCounterFreq	0x5F5E100	//100M
#define TICK_CYCLES		(10*GenCounterFreq/1000) //10ms tick length

int appmain(void) {

	int n, x, y, len;
	x = 20;

	setGenTimerFreq(GenCounterFreq);
	setEL3PhyTimerCV(getPhyCount()+TICK_CYCLES);
	setEL3PhyTimerCtrl(0x01);

	for(n=0; n<16; n++){
		array1[n] += x+array4[n];
		array2[n] = array1[n]*3;
		array2[n+16] = array1[n];
		array3[n] = array2[n] + 2;
		array3[n+16] = array2[n] + 2;
		array3[n+32] = array2[n] + 2;
	}

	enable_uart(UART0);
	write_uart_string(UART0, "Hello\nWorld!");


	while(1)
		;

	return 0;
}
