/**
 * @file sta_smarconfig.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sta_smarconfig.h"
#include "aiio_adapter_include.h"
#include "aiio_log.h"

static aiio_blufi_cb_t blufi_cb = { 0 };
static bool blufi_state = 0;
/**
 * @brief blufi 配网回调
 *
 * @param ssid
 * @param pwd
*/
static void aiio_blufi_connect_success_cb(const char* ssid, const char* pwd)
{
    aiio_log_i("blufi connected Wi-Fi ssid=%s,pwd=%s\r\n", ssid, pwd);
}
/**
 * @brief 初始化blufi 功能
 *
*/
void smartconfig_blufi_init(void)
{
    blufi_cb.wifi_event_cb = aiio_blufi_connect_success_cb;
    aiio_blufi_register_cb(blufi_cb);
}
/**
 * @brief 启动配网
 *
*/
void smartconfig_blufi_start(void)
{
    aiio_blufi_start();
    blufi_state = true;
}
/**
 * @brief 更新配网状态
 *
 * @param blufi_event
*/
void update_smartconfig_blufi_event(int blufi_event)
{
    if (!blufi_state) return;
    aiio_blufi_wifi_event(blufi_event, NULL);
}