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


#define FLASH_RW_SSID_KEY "wifissid"
#define FLASH_RW_PASSWORD_KEY "wifipassword"
#define FLASH_RW_MQ_HOST_KEY "mqtthost"
#define FLASH_RW_MQ_USER_NAME_KEY "mqttusername"
#define FLASH_RW_MQ_PASSWORD_KEY "mqttpassword"


int reset_flash_state = 0;
/**
 * @brief ai_ha_deMsg_flash_nvs_init
 *
 * @param ha_devMsg
*/
void ai_ha_deMsg_flash_nvs_init(ha_devMsg_t* ha_devMsg)
{
    aiio_nvs_init();
    ha_devMsg->dev_mac = malloc(12);
    ha_devMsg->dev_name = malloc(32);
    ha_devMsg->mqtt_host = CONFIG_MQTT_HOST;
    ha_devMsg->mqtt_password = "123456";
    ha_devMsg->mqtt_username = CONFIG_DEVICE_TYPE;
    ha_devMsg->port = CONFIG_MQTT_PORT;
    ha_devMsg->wifi_ssid = malloc(64);
    ha_devMsg->wifi_password = malloc(64);
}
/**
 * @brief 获取连接信息
 *
 * @param ha_devMsg
 * @return int
*/
int homeassistant_get_dev_msg(ha_devMsg_t* ha_devMsg)
{
    if (ha_devMsg==NULL) {
        aiio_log_e("ha_devMsg buff is NULL");
        return 0;
    }
    size_t ha_devMsg_data_len = 0;
    size_t flash_get_len = 0;

    memset(ha_devMsg->wifi_ssid, 0, 64);
    size_t ret = aiio_nvs_get_blob(FLASH_RW_SSID_KEY, ha_devMsg->wifi_ssid, 64, &flash_get_len);
    if (!flash_get_len) {
        aiio_log_e("flash get %s is Not found", FLASH_RW_SSID_KEY);
        return 0;
    }
    aiio_log_d("[nvs get data]%s=%s", FLASH_RW_SSID_KEY, ha_devMsg->wifi_ssid);
    ha_devMsg_data_len += flash_get_len;

    memset(ha_devMsg->wifi_password, 0, 64);
    flash_get_len = 0;
    ret = aiio_nvs_get_blob(FLASH_RW_PASSWORD_KEY, ha_devMsg->wifi_password, 64, &flash_get_len);
    if (!flash_get_len) {
        aiio_log_e("flash get %s is Not found ", FLASH_RW_PASSWORD_KEY);
        return 0;
    }
    aiio_log_d("[nvs get data]%s=%s", FLASH_RW_PASSWORD_KEY, ha_devMsg->wifi_password);
    ha_devMsg_data_len += flash_get_len;

    memset(ha_devMsg->mqtt_host, 0, 64);
    flash_get_len = 0;
    ret = aiio_nvs_get_blob(FLASH_RW_MQ_HOST_KEY, ha_devMsg->mqtt_host, 64, &flash_get_len);
    if (!flash_get_len) {
        aiio_log_e("flash get %s is Not found", FLASH_RW_MQ_HOST_KEY);
        return 0;
    }
    aiio_log_d("[nvs get data]%s=%s", FLASH_RW_MQ_HOST_KEY, ha_devMsg->mqtt_host);
    ha_devMsg_data_len += flash_get_len;

    memset(ha_devMsg->mqtt_password, 0, 512);
    flash_get_len = 0;
    ret = aiio_nvs_get_blob(FLASH_RW_MQ_PASSWORD_KEY, ha_devMsg->mqtt_password, 64, &flash_get_len);
    if (!flash_get_len) {
        aiio_log_e("flash get %s is Not found", FLASH_RW_MQ_PASSWORD_KEY);
        return 0;
    }
    aiio_log_d("[nvs get data]%s=%s", FLASH_RW_MQ_PASSWORD_KEY, ha_devMsg->mqtt_password);

    ha_devMsg_data_len += flash_get_len;
    memset(ha_devMsg->mqtt_username, 0, 128);
    flash_get_len = 0;
    ret = aiio_nvs_get_blob(FLASH_RW_MQ_USER_NAME_KEY, ha_devMsg->mqtt_username, 64, &flash_get_len);
    if (!flash_get_len) {
        aiio_log_e("flash get %s is Not found", FLASH_RW_MQ_USER_NAME_KEY);
        return 0;
    }
    aiio_log_d("[nvs get data]%s=%s", FLASH_RW_MQ_USER_NAME_KEY, ha_devMsg->mqtt_username);
    ha_devMsg_data_len += flash_get_len;

    aiio_log_d("get flash len=%d", ha_devMsg_data_len);
    return (int)ha_devMsg_data_len;
}
/**
 * @brief 保存设备信息
 *
 * @param ha_devMsg
*/
void homeassistant_save_ha_deMsg(ha_devMsg_t* ha_devMsg)
{
    if (ha_devMsg==NULL) {
        aiio_log_e("ha_devMsg buff is NULL");
        return;
    }
    aiio_nvs_erase_key(FLASH_RW_SSID_KEY);
    aiio_nvs_erase_key(FLASH_RW_PASSWORD_KEY);
    aiio_nvs_erase_key(FLASH_RW_MQ_HOST_KEY);
    aiio_nvs_erase_key(FLASH_RW_MQ_PASSWORD_KEY);
    aiio_nvs_erase_key(FLASH_RW_MQ_USER_NAME_KEY);
    char flash_data[64] = { 0 };

    size_t flash_get_len = 0;
    aiio_nvs_err_code_t err = aiio_nvs_set_blob(FLASH_RW_SSID_KEY, ha_devMsg->wifi_ssid, strlen(ha_devMsg->wifi_ssid));
    aiio_log_d("HomeAssistant wifi ssid=%s", ha_devMsg->wifi_ssid);
    if (err!= AIIO_EF_NO_ERR) {
        aiio_log_e("aiio_nvs_set_blob error=%d", err);
        return;
    }
    err = aiio_nvs_set_blob(FLASH_RW_PASSWORD_KEY, ha_devMsg->wifi_password, strlen(ha_devMsg->wifi_password));
    if (err!= AIIO_EF_NO_ERR) {
        aiio_log_e("aiio_nvs_set_blob error=%d", err);
        return;
    }
    err = aiio_nvs_set_blob(FLASH_RW_MQ_HOST_KEY, ha_devMsg->mqtt_host, strlen(ha_devMsg->mqtt_host));
    if (err!= AIIO_EF_NO_ERR) {
        aiio_log_e("aiio_nvs_set_blob error=%d", err);
        return;
    }
    err = aiio_nvs_set_blob(FLASH_RW_MQ_USER_NAME_KEY, ha_devMsg->mqtt_username, strlen(ha_devMsg->mqtt_username));
    if (err!= AIIO_EF_NO_ERR) {
        aiio_log_e("aiio_nvs_set_blob error=%d", err);
        return;
    }
    err = aiio_nvs_set_blob(FLASH_RW_MQ_PASSWORD_KEY, ha_devMsg->mqtt_password, strlen(ha_devMsg->mqtt_password));
    if (err!= AIIO_EF_NO_ERR) {
        aiio_log_e("aiio_nvs_set_blob error=%d", err);
        return;
    }

}
/**
 * @brief
 *
*/
void aiio_flash_clear_ha_deMsg(void)
{
    aiio_nvs_erase_key(FLASH_RW_SSID_KEY);
    aiio_nvs_erase_key(FLASH_RW_PASSWORD_KEY);
    aiio_nvs_erase_key(FLASH_RW_MQ_HOST_KEY);
    aiio_nvs_erase_key(FLASH_RW_MQ_PASSWORD_KEY);
    aiio_nvs_erase_key(FLASH_RW_MQ_USER_NAME_KEY);
}