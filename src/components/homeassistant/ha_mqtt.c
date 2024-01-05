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
#include "ha_mqtt.h"

static ha_devMsg_t homeassistant_dev = { 0 };
