/**
 ********************************************************************************
 * @file      wifi.h
 * @authors   Konrad Staniszewski
 * @brief     All functions related to WiFi
 ********************************************************************************
 */

#pragma once

/**
 * @brief function to initialize WiFi in softAP mode
 */
void wifi_init_softap(void);

/**
 * @brief function to start mDNS service
 */
void start_mdns_service();
