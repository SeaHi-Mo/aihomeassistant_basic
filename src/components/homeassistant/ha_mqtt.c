/**
 * @file ha_mqtt.c
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
#include "led_dev.h"

unsigned char mac[6] = { 0 };
extern unsigned char mac[MAC_LEN];
static aiio_err_t mqtt_event_cb(aiio_mqtt_event_handle_t event);

static aiio_mqtt_client_config_t mqtt_cfg;

discovery_payload_t discovery_payload = { 0 };
/**
 * @brief
 *
 * @param message
 * @param error_code
*/
static void log_error_if_nonzero(const char* message, int error_code)
{
    if (error_code != 0) {
        aiio_log_e("Last error %s: 0x%x", message, error_code);
    }
}
/**
 * @brief
 *
 * @param event
 * @return aiio_err_t
*/
static aiio_err_t mqtt_event_cb(aiio_mqtt_event_handle_t event)
{
    int32_t event_id;
    aiio_mqtt_client_handle_t client = event->client;

    event_id = event->event_id;
    aiio_log_i("Event dispatched, event_id=%d", event_id);
    int msg_id;
    switch ((aiio_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            aiio_log_i("MQTT_EVENT_CONNECTED");
            strcpy(homeAssistant_dev.mqtt_host, mqtt_cfg.host);
            strcpy(homeAssistant_dev.mqtt_username, mqtt_cfg.username);
            strcpy(homeAssistant_dev.mqtt_password, mqtt_cfg.password);
            aiio_log_d("homeAssistant_dev.wifi_ssid=%s", homeAssistant_dev.wifi_ssid);
            aiio_log_d("homeAssistant_dev.wifi_passsword=%s", homeAssistant_dev.wifi_password);
            aiio_log_d("homeAssistant_dev.mqtt_host=%s", homeAssistant_dev.mqtt_host);
            aiio_log_d("homeAssistant_dev.mqtt_username=%s", homeAssistant_dev.mqtt_username);
            aiio_log_d("homeAssistant_dev.mqtt_password=%s", homeAssistant_dev.mqtt_password);
            //连接成功，保存信息
            if (!reset_flash_state)homeassistant_save_ha_deMsg(&homeAssistant_dev);
            //发送HomeAssistant 自动发现的信息
            discovery_payload.name = homeAssistant_dev.dev_name;
            discovery_payload.sw_version = "1.0.0";
            discovery_payload.unique_id = homeAssistant_dev.dev_mac;
            discovery_payload.device.identifiers_len = 1;
            discovery_payload.device.identifiers[0] = homeAssistant_dev.dev_mac;
            discovery_payload.device.name = homeAssistant_dev.dev_name;
            discovery_payload.device.manufacturer = "ai-thinker";
            discovery_payload.device.model = CONFIG_DEVICE_TYPE;
            discovery_payload.device.sw_version = discovery_payload.sw_version;

            homeAssistant_create_device(client, &discovery_payload);
            //发送一次灯的状态
            homeAssistant_send_state(client, &discovery_payload, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            aiio_log_i("MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            aiio_log_i("MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

            // msg_id = aiio_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
            // aiio_log_i("sent publish successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            aiio_log_i("MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            aiio_log_i("MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);

            break;
        case MQTT_EVENT_DATA:
            aiio_log_i("MQTT_EVENT_DATA");
            aiio_log_i("TOPIC=%.*s", event->topic_len, event->topic);
            aiio_log_i("DATA=%.*s", event->data_len, event->data);
            //控制LED
            devLedCtrlValueFromPayload(event->data, event->data_len);
            homeAssistant_send_state(client, &discovery_payload, devLedCtrlGetValue());
            break;
        case MQTT_EVENT_ERROR:
            aiio_log_i("MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->aiio_tls_last_aiio_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->aiio_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno", event->error_handle->aiio_transport_sock_errno);
                aiio_log_i("Last errno string (%s)", strerror(event->error_handle->aiio_transport_sock_errno));
            }
            break;
        default:
            aiio_log_i("Other event id:%d", event->event_id);
            break;
    }
    return AIIO_OK;
}
/**
 * @brief
 *
 * @param devMsg
*/
void ha_dev_mqtt_connenct(ha_devMsg_t* devMsg)
{
    if (devMsg==NULL) {
        aiio_log_e("HomeAssistant dev message buff is NULL!");
        return;
    }
    aiio_mqtt_client_handle_t client;

    mqtt_cfg.event_handle = mqtt_event_cb;
    mqtt_cfg.host = devMsg->mqtt_host;
    mqtt_cfg.client_id = devMsg->dev_mac;
    mqtt_cfg.username = devMsg->mqtt_username;
    mqtt_cfg.port = devMsg->port;
    mqtt_cfg.password = devMsg->mqtt_password;

    memset(&client, 0, sizeof(aiio_mqtt_client_handle_t));
    client = aiio_mqtt_client_init(&mqtt_cfg);
    aiio_mqtt_client_start(client);
}


