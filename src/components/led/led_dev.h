/**
 * @file led_dev.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-12-27
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef LED_DEV_H
#define LED_DEV_H

typedef enum {
    LED_DEV_NETWORKD_STATE_NONE = 0,
    LED_DEV_NETWORKD_STATE_DISCONNECT,
    LED_DEV_NETWORKD_STATE_CONNECTING,
    LED_DEV_NETWORKD_STATE_CONNECTED,
}led_dev_state_t;

void devLedNetWorkdInit(void);
void devLedNetWorkdChangeState(led_dev_state_t led_dev_t);
void devLedCtrlInit(void);
void devLedCtrlValueFromPayload(const char* payload, int payload_len);
int devLedCtrlGetValue(void);
#endif