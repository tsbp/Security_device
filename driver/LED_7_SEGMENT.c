/*
 * LED_7_SEGMENT.c
 *
 *  Created on: 29 вер. 2016
 *      Author: Voodoo
 */
//==============================================================================
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "driver/uart.h"
#include "driver/gpio16.h"
#include "driver/LED_7_SEGMENT.h"
//==============================================================================
uint8 digit[] = {DIG_0, DIG_1, DIG_2, DIG_3, DIG_4,
				 DIG_5, DIG_6, DIG_7, DIG_8, DIG_9,
				 DIG__, DIG_F, DIG_n, DIG_A, DIG_L,
				 DIG_r, DIG_S, DIG_t, DIG_b};
//==============================================================================
void ICACHE_FLASH_ATTR serialSendData(uint16 aWord)
{
	//os_printf("%04x\r\n", lcdWord.word);
	hspi_send_uint16((uint16)aWord);
	hspi_wait_ready();
}
//==============================================================================
void display(uint8 *aDisp, uint8 aBlink)
{
	static int blink = 100;
	static int pos = 0;
	pos++ ;
	if(pos >= 3) pos = 0;

	if (blink) blink--;
	else blink = 200;

	if (aBlink & (1 << pos))
	{
		if (blink > 100) serialSendData(((~digit[aDisp[pos]]) << 8) + (1 << (pos + 1)));
		else  serialSendData(((~digit[10]) << 8) + (1 << (pos + 1)));
	}
	else			serialSendData(((~digit[aDisp[pos]]) << 8) + (1 << (pos + 1)));
}
