/**
 * @file ha_mqtt.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
*/
#ifndef HA_MQTT_H
#define HA_MQTT_H

#define HA_AUTOMATIC_DISCOVERY CONFIG_HA_AUTOMATIC_DISCOVERY  //HomeAssistant 自动发现的前缀
#define HA_DISCOVERY_DEVICE CONFIG_HOMEASSISTANT_MQTT_DISCOVERY //HomeAssistant 实体类型 

typedef struct {
    char* wifi_ssid;
    char* wifi_password;
    char* mqtt_host;
    char* mqtt_username;
    char* mqtt_password;
    char* dev_mac;
    char* dev_name;
    unsigned int port;
}ha_devMsg_t;

typedef struct {
    char* name;
    char* identifiers[32];
    int identifiers_len;
    char* manufacturer;
    char* model;
    char* sw_version;
}ha_msg_dev_t;

typedef struct {
    char* name;
    char* sw_version;
    char* command_topic;
    char* state_topic;
    char* unique_id;
    ha_msg_dev_t device;
}discovery_payload_t;

extern ha_devMsg_t homeAssistant_dev;
void ha_dev_mqtt_connenct(ha_devMsg_t* devMsg);
void homeAssistant_create_device(aiio_mqtt_client_handle_t client, discovery_payload_t* discovery_payload);
void homeAssistant_send_state(aiio_mqtt_client_handle_t client, discovery_payload_t* discovery_payload, int state);
#endif