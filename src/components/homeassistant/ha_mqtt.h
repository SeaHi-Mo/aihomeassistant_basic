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

extern ha_devMsg_t homeAssistant_dev;
void ha_dev_mqtt_connenct(ha_devMsg_t* devMsg);


#endif