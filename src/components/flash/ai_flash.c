/**
 * @file ai_flash.c
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
#include "aiio_adapter_include.h"
#include "ai_flash.h"

static bool aiio_buffer_is_empty_check(char* buffer, uint32_t buffer_len)
{
    if (buffer == NULL)
    {
        aiio_log_e("param err \r\n");
        return false;
    }

    for (uint32_t num = 0; num < buffer_len; num++)
    {
        if (buffer[num] != 0xff)
        {
            return false;
        }
    }

    return true;
}

static bool aiio_wifi_config_data_is_empty_check(ha_devMsg_t* ha_devMsg)
{
    if (config_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return true;
    }

    if (aiio_buffer_is_empty_check(ha_devMsg->wifi_ssid, sizeof(ha_devMsg->wifi_ssid)))               return true;
    if (aiio_buffer_is_empty_check(ha_devMsg->wifi_password, sizeof(ha_devMsg->wifi_password)))         return true;
    if (aiio_buffer_is_empty_check(ha_devMsg->mqtt_host, sizeof(ha_devMsg->mqtt_host)))             return true;
    if (aiio_buffer_is_empty_check(ha_devMsg->mqtt_username, sizeof(ha_devMsg->mqtt_username)))     return true;
    if (aiio_buffer_is_empty_check(ha_devMsg->mqtt_password, sizeof(ha_devMsg->mqtt_password)))              return true;
    if (aiio_buffer_is_empty_check(ha_devMsg->dev_name, sizeof(ha_devMsg->dev_name)))                 return true;

    if (aiio_strlen(ha_devMsg->wifi_ssid) == 0  ||
        aiio_strlen(ha_devMsg->wifi_password) == 0 || aiio_strlen(ha_devMsg->mqtt_host) == 0 ||
        aiio_strlen(ha_devMsg->mqtt_username) == 0 || aiio_strlen(ha_devMsg->mqtt_password) == 0 || aiio_strlen(ha_devMsg->dev_name) == 0)
    {
        return true;
    }

    return false;
}

int homeassistant_get_dev_msg(ha_devMsg_t* ha_devMsg)
{

}
