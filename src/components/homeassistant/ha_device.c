/**
 * @file ha_device.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-10
 *
 * @copyright Copyright (c) 2024
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aiio_adapter_include.h"
#include "ai_flash.h"

static char* payload[1024];
/**
 * @brief 创建实体，并向HomeAssistant 发送发现信息
 *
 * @param client
 * @param discovery_topic
 * @param discovery_payload
*/
void homeAssistant_create_device(aiio_mqtt_client_handle_t client, discovery_payload_t* discovery_payload)
{
    if (client==NULL) {
        aiio_log_e("MQTT client is null,client is not init");
        return;
    }

    char* mqtt_discovery_topic = malloc(256);


    if (discovery_payload->command_topic==NULL) {
        discovery_payload->command_topic = malloc(256);
        memset(discovery_payload->command_topic, 0, 256);
        sprintf(discovery_payload->command_topic, "%s/%s/%s/set", HA_AUTOMATIC_DISCOVERY, HA_DISCOVERY_DEVICE, homeAssistant_dev.dev_mac);

    }

    if (discovery_payload->state_topic==NULL) {
        discovery_payload->state_topic = malloc(256);
        memset(discovery_payload->state_topic, 0, 256);
        sprintf(discovery_payload->state_topic, "%s/%s/%s/state", HA_AUTOMATIC_DISCOVERY, HA_DISCOVERY_DEVICE, homeAssistant_dev.dev_mac);
    }
    sprintf(mqtt_discovery_topic, "%s/%s/%s/config", HA_AUTOMATIC_DISCOVERY, HA_DISCOVERY_DEVICE, homeAssistant_dev.dev_mac);

    if (discovery_payload->name==NULL
        ||discovery_payload->sw_version==NULL
        ||discovery_payload->unique_id==NULL
        ||discovery_payload->device.identifiers_len==0
        ||discovery_payload->device.name==NULL) {
        aiio_log_e("HomeAssistant param errpr");
        return;
    }
    cJSON* root = cJSON_CreateObject();
    if (root==NULL) {
        aiio_log_e("cjson root Create error");
        return;
    }

    cJSON_AddStringToObject(root, "name", discovery_payload->name);
    cJSON_AddStringToObject(root, "command_topic", discovery_payload->command_topic);
    cJSON_AddStringToObject(root, "state_topic", discovery_payload->state_topic);
    cJSON_AddStringToObject(root, "unique_id", discovery_payload->unique_id);
    cJSON* device = cJSON_CreateObject();
    cJSON* identifiers_arr = cJSON_AddArrayToObject(device, "identifiers");
    for (size_t i = 0; i < discovery_payload->device.identifiers_len; i++)
    {
        cJSON_AddItemToArray(identifiers_arr, cJSON_CreateString(discovery_payload->device.identifiers[i]));
    }
    cJSON_AddStringToObject(device, "name", discovery_payload->device.name);
    //添加产商名称
    if (discovery_payload->device.manufacturer!=NULL) cJSON_AddStringToObject(device, "manufacturer", discovery_payload->device.manufacturer);
    //添加设备型号
    if (discovery_payload->device.model!=NULL) cJSON_AddStringToObject(device, "model", discovery_payload->device.model);
    //添加设备软件版本
    if (discovery_payload->device.sw_version!=NULL) cJSON_AddStringToObject(device, "sw_version", discovery_payload->device.sw_version);

    cJSON_AddItemToObject(root, "device", device);
    char* json_data = cJSON_PrintUnformatted(root);

    int msg_id = aiio_mqtt_client_publish(client, mqtt_discovery_topic, json_data, strlen(json_data), 0, 1);
    msg_id = aiio_mqtt_client_subscribe(client, discovery_payload->command_topic, 0);
    free(json_data);
    cJSON_Delete(root);
    free(mqtt_discovery_topic);
}
/**
 * @brief 发送状态，可向HomeAssistant 开关状态
 *
 * @param client
 * @param discovery_payload
 * @param state
*/
void homeAssistant_send_state(aiio_mqtt_client_handle_t client, discovery_payload_t* discovery_payload, int state)
{
    if (client==NULL|| discovery_payload->state_topic==NULL) {
        aiio_log_e("MQTT client is null,client is not init");
        return;
    }

    int msg_id = aiio_mqtt_client_publish(client, discovery_payload->state_topic, state?"ON":"OFF", state?2:3, 0, 0);
}