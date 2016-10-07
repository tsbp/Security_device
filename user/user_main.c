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
#include "driver/wifi.h"
#include "mqtt.h"
#include "debug.h"
#include "mem.h"
//============================================================================================================================
extern int ets_uart_printf(const char *fmt, ...);
int (*console_printf)(const char *fmt, ...) = ets_uart_printf;

#define GPIO_LED_PIN 0

#define DISPLAY_PERIOD		(5) // in msec
#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

static volatile os_timer_t display_timer;
static void  display_loop(os_event_t *events);

#define LOOP_PERIOD		(1000) // in msec
static volatile os_timer_t loop_timer;
static void  loop(os_event_t *events);

#define PLOT_INTERVAL   (600)
static struct ip_info ipConfig;

typedef void (*WifiCallback)(uint8_t);
WifiCallback wifiCb;

MQTT_Client mqttClient;
uint8 alarm_on = 1;
//==============================================================================
uint8 blink = 0;
uint8 disp[3] = {11, 11, 11};
uint8 disp_off[3] = {0, 11, 11};
uint8 disp_ALR[3] = {13, 14, 15};
uint8 disp_STB[3]  ={16, 17, 18};
//======================= GPIO interrupt callback =======================================================
void ICACHE_FLASH_ATTR button_intr_callback(unsigned pin, unsigned level)
{
	ets_uart_printf("RESET BUTTON PRESSED!!!\r\n");

	MQTT_Publish(&mqttClient, "hello", "my data", 7, 0, 0);

}
//======================= GPIO init function ============================================================
uint8_t in_pin = 4;
uint8_t button_pin = 6;
//==============================================================================
void ICACHE_FLASH_ATTR button_init(void)
{
	GPIO_INT_TYPE gpio_type;
	gpio_type = GPIO_PIN_INTR_NEGEDGE;

	//==== input pin  ====
	set_gpio_mode(in_pin, GPIO_PULLUP, GPIO_INT);

	//==== bitton pin ====
	if (set_gpio_mode(button_pin, GPIO_PULLUP, GPIO_INT))
		{
			if (gpio_intr_init(button_pin, gpio_type))
				gpio_intr_attach(button_intr_callback);
			ets_uart_printf("button init!!!\r\n");
		}

	ets_uart_printf("input init!!!\r\n");

}
//======================= Main code function ============================================================
void copyScreen(uint8 *aInBuf, uint8 *aOutBuf)
{
	int i;
	for(i = 0; i < 3; i++) aInBuf[i] = aOutBuf[i];
}
//=======================
void ICACHE_FLASH_ATTR loop(os_event_t *events)
{
//	wifi_station_get_connect_status();
//	wifi_get_ip_info(STATION_IF, &ipConfig);
//	ets_uart_printf(/*IPSTR, IP2STR*/"IP  is %d \r\n", (ipConfig.ip.addr));
	//ets_uart_printf("working \r\n");

//	a ^= 1;
//	gpio_write(1, a);

	int stt = gpio_read(in_pin);

	uint8 *scr;
	uint8 data[2];

    if(stt) { scr = disp_ALR; blink = 7; data[0] = 'O';} //	display(disp_off, 0);
    else 	{ scr = disp_STB; blink = 0; data[0] = 'C';} //display(disp_on, 7);



    if(!alarm_on) { scr = disp_off;  blink = 0; data[1] = 'N';}
    else data[1] = 'A';

    MQTT_Publish(&mqttClient, "stt", data, 2, 0, 0);

    copyScreen(disp, scr);

//    if(!gpio_read(button_pin)) display(disp_hz, 5);
//
//	gpio_write(1, stt);
}
//======================= Main code function ============================================================

void ICACHE_FLASH_ATTR display_loop(os_event_t *events)
{
	display(disp, blink);
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

	// Start display timer
	os_timer_disarm(&display_timer);
	os_timer_setfn(&display_timer, (os_timer_func_t *) display_loop, NULL);
	os_timer_arm(&display_timer, DISPLAY_PERIOD, true);

}
//=============================================================================================
void wifiConnectCb(uint8_t status)
{
	ets_uart_printf(/*IPSTR, IP2STR*/"wifi call back\r\n");
	if(status == STATION_GOT_IP){
		MQTT_Connect(&mqttClient);
	} else {
		MQTT_Disconnect(&mqttClient);
	}
}
//=============================================================================================
void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Connected\r\n");
	MQTT_Subscribe(client, "cfg", 0);
//	MQTT_Subscribe(client, "/mqtt/topic/1", 1);
//	MQTT_Subscribe(client, "/mqtt/topic/2", 2);
//
//	MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);
//	MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);
//	MQTT_Publish(client, "/mqtt/topic/2", "hello2", 6, 2, 0);

}
//=============================================================================================
void mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Disconnected\r\n");
}
//=============================================================================================
void mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	INFO("MQTT: Published\r\n");
}
//=============================================================================================
void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	ets_uart_printf("Receive topic: %s, data: %s !\r\n", topicBuf, dataBuf);

//	if		(dataBuf[4] =='O' && dataBuf[5] =='N')  alarm_on = 1;
//	else if (dataBuf[4] =='O' && dataBuf[5] =='F')  alarm_on = 0;



	if		(!strcmp(dataBuf, "ALR_ON"))  alarm_on = 1;
		else if (!strcmp(dataBuf, "ALR_OFF")) alarm_on = 0;

	ets_uart_printf("alarm_on: %d\r\n", alarm_on);

	os_free(topicBuf);
	os_free(dataBuf);
}
//========================== Init function  =============================================================
//
uint8 mac[8];
void ICACHE_FLASH_ATTR user_init(void)
{

	uart_init(BIT_RATE_115200, BIT_RATE_115200);
	os_delay_us(1000000);
	ets_uart_printf("System init...\r\n");

	wifi_station_disconnect();
	wifi_station_set_auto_connect(0);

	wifi_get_macaddr(STATION_IF, mac);
	ets_uart_printf(MACSTR, MAC2STR(mac));
	ets_uart_printf("\r\n");
//	wifi_get_macaddr(SOFTAP_IF, mac);
//	ets_uart_printf(MACSTR, MAC2STR(mac));
//
//	//system_update_cpu_freq(160);


	MQTT_InitConnection(&mqttClient, "m20.cloudmqtt.com", 13363,
			0);
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, "voodoo140382", "hhxlerts",
			"JC-QW1mRKu5W", 120, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

	WIFI_Connect("TPLINK_RPCR", "60673251", wifiConnectCb);


	// Start setup timer
	os_timer_disarm(&loop_timer);
	os_timer_setfn(&loop_timer, (os_timer_func_t *) setup, NULL);
	os_timer_arm(&loop_timer, LOOP_PERIOD * 2, false);
}
