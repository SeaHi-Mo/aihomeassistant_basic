/**
 * @file wifi_sta.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
*/
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "aiio_adapter_include.h"
#include "led_dev.h"
#include "sta_smarconfig.h"
static int32_t ret = 0;
static int32_t rssi = 0;
static int32_t state = 0;
static bool wifi_is_connect;
int wifi_config_start = false;
uint8_t mac[MAC_LEN];

aiio_wifi_sta_connect_ind_stat_info_t wifi_ind_stat;
/**
 * @brief cb_wifi_event
 *    连接WiFi的回调
 * @param event
 * @param data
*/
static void cb_wifi_event(aiio_input_event_t* event, void* data)
{
    int32_t ret = 0;
    // uint8_t bssid[6] = {0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5};
    switch (event->code) {
        case AIIO_WIFI_EVENT_WIFI_READY:
            aiio_log_d("<<<<<<<<<  WIFI INIT OK <<<<<<<<<<");
            break;
        case AIIO_WIFI_EVENT_SCAN_DONE:
            aiio_log_d("<<<<<<<<<  SCAN DONE OK <<<<<<<<<<");

            break;
        case AIIO_WIFI_EVENT_STA_CONNECTING:
            aiio_log_d("<<<<<<<<< STA_CONNECTING <<<<<<<<<<<");
            devLedNetWorkdChangeState(LED_DEV_NETWORKD_STATE_CONNECTING);

            break;
        case AIIO_WIFI_EVENT_STA_CONNECTED:
            aiio_log_d("<<<<<<<<< CONNECTED<<<<<<<<<<<");
            update_smartconfig_blufi_event(AIIO_WIFI_EVENT_STA_CONNECTED);
            break;
        case AIIO_WIFI_EVENT_STA_DISCONNECTED:
            aiio_log_d("<<<<<<<<<  DISCONNECTED <<<<<<<<<<");
            devLedNetWorkdChangeState(LED_DEV_NETWORKD_STATE_DISCONNECT);
            update_smartconfig_blufi_event(AIIO_WIFI_EVENT_STA_DISCONNECTED);

            if (event->value == AIIO_WLAN_FW_4WAY_HANDSHAKE_ERROR_PSK_TIMEOUT_FAILURE ||
                    event->value == AIIO_WLAN_FW_AUTH_OR_ASSOC_RESPONSE_TIMEOUT_FAILURE) {
                //connect timeout
                aiio_log_d("connect timeout");
            }
            else if (event->value == AIIO_WLAN_FW_4WAY_HANDSHAKE_TX_DEAUTH_FRAME_TRANSMIT_FAILURE ||
                   event->value == AIIO_WLAN_FW_4WAY_HANDSHAKE_TX_DEAUTH_FRAME_ALLOCATE_FAIILURE ||
                   event->value == AIIO_WLAN_FW_DEAUTH_BY_AP_WHEN_NOT_CONNECTION) {
                //password error
                aiio_log_d("password error");
            }
            else if (event->value == AIIO_WLAN_FW_SCAN_NO_BSSID_AND_CHANNEL) {
                //not found AP
                aiio_log_d("not found AP");
            }
            else if ((event->value == AIIO_WLAN_FW_DEAUTH_BY_AP_WHEN_CONNECTION) || (event->value == AIIO_WLAN_FW_DISCONNECT_BY_USER_WITH_DEAUTH)) {
                //wifi disconnect
                aiio_log_d("wifi disconnect");
            }
            else {
                //connect error
                aiio_log_d("connect error");
            }
            break;
        case AIIO_WIFI_EVENT_STA_GOT_IP:
        {
            aiio_log_d("<<<<<<<<< CONNECTED GOT IP <<<<<<<<<<<");
            devLedNetWorkdChangeState(LED_DEV_NETWORKD_STATE_CONNECTED);
            update_smartconfig_blufi_event(AIIO_WIFI_EVENT_STA_GOT_IP);
            wifi_is_connect = true;
            aiio_wifi_rssi_get(&rssi);
            aiio_log_d("wifi cur_rssi = %d!!", rssi);
            aiio_wifi_sta_mac_get(mac);
            aiio_log_d("wifi mac = %02x%02x%02x%02x%02x%02x!!", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            aiio_wifi_sta_connect_ind_stat_get(&wifi_ind_stat);
            aiio_log_d("wifi sta_bssid = %02x%02x%02x%02x%02x%02x", wifi_ind_stat.bssid[0], wifi_ind_stat.bssid[1], wifi_ind_stat.bssid[2], wifi_ind_stat.bssid[3], wifi_ind_stat.bssid[4], wifi_ind_stat.bssid[5]);

        }
        break;
        default:
            break;
    }
}
/**
 * @brief
 *
*/
void staWiFiInit(void)
{
    aiio_wifi_register_event_cb(cb_wifi_event);
    int ret = aiio_wifi_init();
    wifi_is_connect = false;
}
/**
 * @brief staWiFiConnect
 *
 * @param ssid
 * @param password
*/
void staWiFiConnect(char* ssid, char* password)
{
    if (wifi_is_connect)
        aiio_wifi_disconnect();
    aiio_wifi_set_mode(AIIO_WIFI_MODE_STA);
    aiio_wifi_config_t wifi_sta_config;
    memset(wifi_sta_config.sta.ssid, 0, sizeof(wifi_sta_config.sta.ssid));
    strcpy((char*)wifi_sta_config.sta.ssid, ssid);
    if (password)
    {
        memset(wifi_sta_config.sta.password, 0, sizeof(wifi_sta_config.sta.password));
        memcpy(wifi_sta_config.sta.password, password, strlen(password));
    }
    wifi_sta_config.sta.channel = 0;
    wifi_sta_config.sta.use_dhcp = 1;
    wifi_sta_config.sta.flags = 0;
    aiio_log_d("ssid = %s \r\n", wifi_sta_config.sta.ssid);
    aiio_log_d("password = %s \r\n", wifi_sta_config.sta.password);
    aiio_wifi_set_config(AIIO_WIFI_IF_STA, &wifi_sta_config);
    aiio_wifi_start();
    devLedNetWorkdChangeState(LED_DEV_NETWORKD_STATE_DISCONNECT);
    wifi_is_connect = false;
}
/**
 * @brief
 *
*/
void staWiFiDisconnect(void)
{
    aiio_wifi_disconnect();
    devLedNetWorkdChangeState(LED_DEV_NETWORKD_STATE_DISCONNECT);
    aiio_os_tick_dealy(aiio_os_ms2tick(2000));

}
