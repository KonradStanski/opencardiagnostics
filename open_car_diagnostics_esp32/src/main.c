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
#include "udp_server.h"
#include "wifi.h"
#include <string.h>
#include "esp_websocket_client.h"

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

   // Initialize UDP Server
   ESP_LOGI(TAG, "STARTING UDP SERVER");
   xTaskCreate(udp_server_task, "udp_server", 4096, NULL, 5, NULL);

    // Start mDNS Service
    ESP_LOGI(TAG, "STARTING mDNS SERVICE");
    start_mdns_service();
}
