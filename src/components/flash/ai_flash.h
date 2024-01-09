/**
 * @file ai_flash.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
*/
#ifndef AI_FLASH_H
#define AI_FLASH_H
#include "ha_mqtt.h"

extern int reset_flash_state;

void ai_ha_deMsg_flash_nvs_init(ha_devMsg_t* ha_devMsg);
int homeassistant_get_dev_msg(ha_devMsg_t* ha_devMsg);
void homeassistant_save_ha_deMsg(ha_devMsg_t* ha_devMsg);
void aiio_flash_clear_ha_deMsg(void);

#endif