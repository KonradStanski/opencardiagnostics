/**
 ********************************************************************************
 * Open Car Diagnostics
 * @file      main.c
 * @brief     This file contains the main function for the project
 *            It initializes the NVS, WiFi, and UDP Server
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "websocket_server.h"
#include "wifi.h"
#include <esp_wifi.h>
#include <string.h>

/************************************
 * STATIC VARIABLES AND DEFINES
 ************************************/
static const char* TAG = "MAIN";

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void app_main(void) {
   // Initialize Non Volatile Storage (NVS)
   esp_err_t ret = nvs_flash_init();
   if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
   }
   ESP_ERROR_CHECK(ret);

   // Initialize Event Loop
   ESP_ERROR_CHECK(esp_event_loop_create_default());

   // Initialize WiFi
   ESP_LOGI(TAG, "STARTING WIFI");
   wifi_init_softap();

   // Start mDNS Service
   ESP_LOGI(TAG, "STARTING mDNS SERVICE");
   start_mdns_service();

   // setup websocket server
   init_websocket_server();
}
