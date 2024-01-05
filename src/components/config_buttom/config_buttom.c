/**
 * @file config_buttom.c
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
#include "sta_smarconfig.h"
#include "aiio_log.h"
#define CONFIGBUTTOM_PIN AIIO_GPIO_NUM_8
#define CONFIGBUTTOM_LONG_PRESS_TIME 2000

static aiio_os_thread_handle_t* configButtom_task;
/**
 * @brief configButtomLongPressCb
 *    长按识别任务
 * @param arg
*/
static void configButtomLongPressCb(void* arg)
{
    uint8_t config_buttom_press_cout = 0;
    while (1)
    {
        if (aiio_hal_gpio_get(0, CONFIGBUTTOM_PIN)) {
            aiio_os_tick_dealy(aiio_os_ms2tick(10));

            while (aiio_hal_gpio_get(0, CONFIGBUTTOM_PIN))
            {
                config_buttom_press_cout++;
                aiio_os_tick_dealy(aiio_os_ms2tick(100));
                if (config_buttom_press_cout>=(2000/100)) {
                    /**** 按键长按测试打印 ****/
                    aiio_log_d(">>> onfig buttom long press <<<");
                    smartconfig_blufi_start();
                    config_buttom_press_cout = 0;
                }
            }
        }
        config_buttom_press_cout = 0;
        aiio_os_tick_dealy(aiio_os_ms2tick(100));
    }
}
/**
 * @brief
 *
*/
void configButtomInit(void)
{
    aiio_hal_gpio_init(0, CONFIGBUTTOM_PIN);
    aiio_hal_gpio_pin_pull_set(0, CONFIGBUTTOM_PIN, AIIO_GPIO_PULL_DOWN);
    aiio_hal_gpio_pin_direction_set(0, CONFIGBUTTOM_PIN, AIIO_GPIO_INPUT);
    aiio_os_thread_create(&configButtom_task, "config_buttom_task", configButtomLongPressCb, 1024*2, NULL, 14);
}

