/**
 * @file wifi_sta.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-01-04
 *
 * @copyright Copyright (c) 2024
 *
*/
#ifndef WIFI_STA_H
#define WIFI_STA_H




void staWiFiInit(void);
void staWiFiConnect(char* ssid, char* password);

void staWiFiDisconnect(void);
#endif