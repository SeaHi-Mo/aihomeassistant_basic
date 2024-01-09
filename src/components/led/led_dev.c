/**
 * @file led_dev.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aiio_adapter_include.h"
#include "led_dev.h"
#define DEV_LED_NETWORKD_BLUE CONFIG_NETWORKD_LED
#define DEV_LED_CTRL_GREEN CONFIG_CTRL_LED

static led_dev_state_t led_dev_state = LED_DEV_NETWORKD_STATE_NONE;
static int led_dev_ctrl_state = 0;

/**
 * @brief
 *
 * @param arg
*/
static void led_dev_run_task(void* arg)
{
    __uint8_t delay_ms = 0;
    while (1)
    {
        switch (led_dev_state)
        {
            case LED_DEV_NETWORKD_STATE_DISCONNECT:
                /*  断开连接 */
                delay_ms = 100;
                break;
            case LED_DEV_NETWORKD_STATE_CONNECTING:
                /* 正在配网 */
                delay_ms = 500;
                break;
            case LED_DEV_NETWORKD_STATE_CONNECTED:
                /*  连接成功*/
                delay_ms = 80;
                aiio_os_tick_dealy(aiio_os_ms2tick(2000));
                aiio_hal_gpio_set(0, DEV_LED_NETWORKD_BLUE, 1);
                aiio_os_tick_dealy(aiio_os_ms2tick(delay_ms));
                aiio_hal_gpio_set(0, DEV_LED_NETWORKD_BLUE, 0);
                aiio_os_tick_dealy(aiio_os_ms2tick(delay_ms));
                break;
            default:
                aiio_os_tick_dealy(aiio_os_ms2tick(50));
                continue;
        }
        aiio_hal_gpio_set(0, DEV_LED_NETWORKD_BLUE, 1);
        aiio_os_tick_dealy(aiio_os_ms2tick(delay_ms));
        aiio_hal_gpio_set(0, DEV_LED_NETWORKD_BLUE, 0);
        aiio_os_tick_dealy(aiio_os_ms2tick(delay_ms));
    }

}
/**
 * @brief devLedNetWorkdInit 网络灯初始化
 *
*/
void devLedNetWorkdInit(void)
{
    aiio_hal_gpio_init(0, DEV_LED_NETWORKD_BLUE);
    aiio_hal_gpio_pin_direction_set(0, DEV_LED_NETWORKD_BLUE, AIIO_GPIO_OUTPUT);
    aiio_hal_gpio_set(0, DEV_LED_NETWORKD_BLUE, 0);
    aiio_os_thread_create(NULL, "led_state_task", led_dev_run_task, 1024, NULL, 15);
    led_dev_state = LED_DEV_NETWORKD_STATE_DISCONNECT;
    aiio_log_d("netWorkd led init sucess!");
}
/**
 * @brief devLedNetWorkdChangeState
 *    更新灯的闪烁状态
 * @param led_dev_t
*/
void devLedNetWorkdChangeState(led_dev_state_t led_dev_t)
{
    led_dev_state = led_dev_t;
}
/**
 * @brief 受控灯初始化
 *
*/
void devLedCtrlInit(void)
{
    aiio_hal_gpio_init(0, DEV_LED_CTRL_GREEN);
    aiio_hal_gpio_pin_direction_set(0, DEV_LED_CTRL_GREEN, AIIO_GPIO_OUTPUT);
    aiio_hal_gpio_set(0, DEV_LED_CTRL_GREEN, 0);
}

void devLedCtrlValueFromPayload(char* payload)
{
    if (payload==NULL) {
        aiio_log_e("payload is NULL");
        return;
    }
    cJSON* root = cJSON_Parse(payload);
    if (root==NULL) {
        aiio_log_e("[%s]payload is no json", payload);
        return;
    }
    cJSON* ctrl = cJSON_GetObjectItem(root, "control");
    cJSON* msgid_bool = cJSON_GetObjectItem(ctrl, "1");
    if (msgid_bool->valueint) aiio_hal_gpio_set(0, DEV_LED_CTRL_GREEN, 1);
    else  aiio_hal_gpio_set(0, DEV_LED_CTRL_GREEN, 0);

    led_dev_ctrl_state = msgid_bool->valueint;
    cJSON_Delete(root);
}

int devLedCtrlGetValue(void)
{
    return led_dev_ctrl_state;
}