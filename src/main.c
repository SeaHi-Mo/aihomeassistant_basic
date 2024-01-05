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

    while (1)
    {
        aiio_os_tick_dealy(aiio_os_ms2tick(2000));
    }
}

