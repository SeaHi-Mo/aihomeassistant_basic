/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-05
 *
 * @copyright Copyright (c) 2024
 *
*/
#include "aiio_adapter_include.h"
#include "led_dev.h"
#include "config_buttom.h"
#include "wifi_sta.h"
#include "sta_smarconfig.h"
#include "ai_flash.h"

ha_devMsg_t homeAssistant_dev = { 0 };

void aiio_main(void* params)
{

    //初始化网络灯
    devLedNetWorkdInit();
    // 配网按键初始化
    configButtomInit();
    //WiFi 连接测试
    staWiFiInit();
    smartconfig_blufi_init();
    aiio_blufi_set_ble_name("HA-dev_blufi");
    //读取flash 内部信息
    ai_ha_deMsg_flash_nvs_init(&homeAssistant_dev);

    int flash_msg_len = homeassistant_get_dev_msg(&homeAssistant_dev);
    if (flash_msg_len>0) {
        if (strlen(homeAssistant_dev.wifi_ssid)!=0&&strlen(homeAssistant_dev.mqtt_host)!=0)
        {
            staWiFiConnect(homeAssistant_dev.wifi_ssid, homeAssistant_dev.wifi_password);
            reset_flash_state = true;
        }
        else {
            smartconfig_blufi_start();
            reset_flash_state = false;
        }
    }
    else {
        smartconfig_blufi_start();
        reset_flash_state = false;
    }

    while (1)
    {
        aiio_os_tick_dealy(aiio_os_ms2tick(2000));
    }
}

