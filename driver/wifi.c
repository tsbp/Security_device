//============================================================================================================================
#include "ets_sys.h"
#include "osapi.h"
#include "c_types.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
//============================================================================================================================
//void setup_wifi_ap_mode(void)
//{
//	wifi_set_opmode((wifi_get_opmode()|STATIONAP_MODE)&STATIONAP_MODE);
//	struct softap_config apconfig;
//	if(wifi_softap_get_config(&apconfig))
//	{
//		wifi_softap_dhcps_stop();
//		os_memset(apconfig.ssid, 0, sizeof(apconfig.ssid));
//		os_memset(apconfig.password, 0, sizeof(apconfig.password));
//		apconfig.ssid_len = os_sprintf(apconfig.ssid, configs.hwSettings.wifi.SSID);//"HA-HA-HA");
//		os_sprintf(apconfig.password, "%s", configs.hwSettings.wifi.SSID_PASS);//"qwertyuiop");
//		apconfig.authmode = configs.hwSettings.wifi.auth;//AUTH_OPEN;//AUTH_WPA_WPA2_PSK;
//		apconfig.ssid_hidden = 0;
//		apconfig.channel = 7;
//		apconfig.max_connection = 4;
//		if(!wifi_softap_set_config(&apconfig))
//		{
//			//#if DEBUG_LEVEL > 0
//			ets_uart_printf("ESP8266 not set AP config!\r\n");
//			//#endif
//		};
//		struct ip_info ipinfo;
//		wifi_get_ip_info(SOFTAP_IF, &ipinfo);
//		IP4_ADDR(&ipinfo.ip, 192, 168, 4, 100);
//		IP4_ADDR(&ipinfo.gw, 192, 168, 4, 100);
//		IP4_ADDR(&ipinfo.netmask, 255, 255, 255, 0);
//		wifi_set_ip_info(SOFTAP_IF, &ipinfo);
//		wifi_softap_dhcps_start();
//	}
//	//#if DEBUG_LEVEL > 0
//	ets_uart_printf("ESP8266 in AP mode configured.\r\n");
//	//#endif
//}
//============================================================================================================================
void ICACHE_FLASH_ATTR setup_wifi_st_mode(void)
{
	wifi_set_opmode(STATION_MODE);
	struct station_config stconfig;
	wifi_station_disconnect();
	wifi_station_dhcpc_stop();
	if(wifi_station_get_config(&stconfig))
	{
		os_memset(stconfig.ssid, 0, sizeof(stconfig.ssid));
		os_memset(stconfig.password, 0, sizeof(stconfig.password));
		os_sprintf(stconfig.ssid, "%s", "TPLINK_RPCR");
		os_sprintf(stconfig.password, "%s", "60673251");
		if(!wifi_station_set_config(&stconfig))
		{
			ets_uart_printf("ESP8266 not set station config!\r\n");
		}
	}
	wifi_station_connect();
	wifi_station_dhcpc_start();
	wifi_station_set_auto_connect(1);
	ets_uart_printf("ESP8266 in STA mode configured.\r\n");
}
