//============================================================================================================================
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "driver/gpio16.h"
#include "driver/LED_7_SEGMENT.h"
//============================================================================================================================
extern int ets_uart_printf(const char *fmt, ...);
int (*console_printf)(const char *fmt, ...) = ets_uart_printf;

#define GPIO_LED_PIN 0

#define LOOP_PERIOD		(5) // in msec
#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

static volatile os_timer_t loop_timer;
static void  loop(os_event_t *events);
uint8 swap = 0;

#define PLOT_INTERVAL   (600)
static struct ip_info ipConfig;

//======================= GPIO interrupt callback =======================================================
void ICACHE_FLASH_ATTR button_intr_callback(unsigned pin, unsigned level)
{
	ets_uart_printf("RESET BUTTON PRESSED!!!\r\n");
}
//======================= GPIO init function ============================================================
uint8_t factory_reset_pin = 4;
//==============================================================================
void ICACHE_FLASH_ATTR button_init(void)
{
	GPIO_INT_TYPE gpio_type;
	gpio_type = GPIO_PIN_INTR_NEGEDGE;
	if (set_gpio_mode(factory_reset_pin, GPIO_PULLUP, GPIO_INT))
	{
		if (gpio_intr_init(factory_reset_pin, gpio_type))
			gpio_intr_attach(button_intr_callback);
	}
	ets_uart_printf("btn init!!!\r\n");
}

//======================= Main code function ============================================================
uint8 disp_off[3] = {0,11,11}, a = 0;
uint8 disp_on[3]  = {0,12,10};
//==============================================================================
void ICACHE_FLASH_ATTR loop(os_event_t *events)
{
//	wifi_station_get_connect_status();
//	wifi_get_ip_info(STATION_IF, &ipConfig);
//	ets_uart_printf(/*IPSTR, IP2STR*/"IP  is %d \r\n", (ipConfig.ip.addr));
	//ets_uart_printf("working \r\n");

//	a ^= 1;
//	gpio_write(1, a);

	if(gpio_read(factory_reset_pin)) 	display(disp_off);
	else 								display(disp_on);

}
//==============================================================================
void ICACHE_FLASH_ATTR setup(void)
{
	//set_gpio_mode(1,GPIO_PULLUP, GPIO_OUTPUT);
	button_init();
	hspi_init();
	// Start loop timer
	os_timer_disarm(&loop_timer);
	os_timer_setfn(&loop_timer, (os_timer_func_t *) loop, NULL);
	os_timer_arm(&loop_timer, LOOP_PERIOD, true);

}
//========================== Init function  =============================================================
//
uint8 mac[8];
void ICACHE_FLASH_ATTR user_init(void)
{

	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);
	ets_uart_printf("System init...\r\n");

//	wifi_station_disconnect();
//	wifi_station_set_auto_connect(0);
//	//bool wifi_get_macaddr(uint8 if_index, uint8 *macaddr);
//
//	wifi_get_macaddr(STATION_IF, mac);
//	ets_uart_printf(MACSTR, MAC2STR(mac));
//	ets_uart_printf("\r\n");
//	wifi_get_macaddr(SOFTAP_IF, mac);
//	ets_uart_printf(MACSTR, MAC2STR(mac));
//
//	//system_update_cpu_freq(160);
//    setup_wifi_st_mode();

	// Start setup timer
	os_timer_disarm(&loop_timer);
	os_timer_setfn(&loop_timer, (os_timer_func_t *) setup, NULL);
	os_timer_arm(&loop_timer, LOOP_PERIOD * 2, false);
}
