/**
 ********************************************************************************
 * @file      wifi.c
 * @authors   Konrad Staniszewski
 * @brief     This file contains all functions related to WiFi
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "mdns.h"
#include <string.h>

/************************************
 * STATIC VARIABLES AND DEFINES
 ************************************/
#define EXAMPLE_ESP_WIFI_SSID "OpenCarDiagnostics"
#define EXAMPLE_ESP_WIFI_PASS "PASSWORD"
#define EXAMPLE_ESP_WIFI_CHANNEL 0
#define EXAMPLE_MAX_STA_CONN 1

static const char* TAG = "WIFI_SOFTAP";
static const char* MDNS_INSTANCE = "Open Car Diagnostics ";

/************************************
 * STATIC FUNCTIONS
 ************************************/
/**
 * @brief   Event handler for WiFi events
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
   if(event_id == WIFI_EVENT_AP_STACONNECTED) {
      wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*)event_data;
      // print mac address
      ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
   } else if(event_id == WIFI_EVENT_AP_STADISCONNECTED) {
      wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*)event_data;
      ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d", MAC2STR(event->mac), event->aid);
   }
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
/**
 * @brief function to initialize WiFi in softAP mode
 */
void wifi_init_softap(void) {
   // Init underlying TCP/IP stack
   ESP_ERROR_CHECK(esp_netif_init());
   // create default wifi ap for some reason
   esp_netif_create_default_wifi_ap();
   // get default wifi config
   wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
   // init wifi with default config
   ESP_ERROR_CHECK(esp_wifi_init(&cfg));
   // register event handler
   ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
   // custom wifi config
   wifi_config_t wifi_config = {
       .ap = {.ssid = EXAMPLE_ESP_WIFI_SSID,
              .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
              .channel = EXAMPLE_ESP_WIFI_CHANNEL,
              .password = EXAMPLE_ESP_WIFI_PASS,
              .max_connection = EXAMPLE_MAX_STA_CONN,
              .authmode = WIFI_AUTH_WPA_WPA2_PSK},
   };
   // disable password if password length 0
   if(strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
      wifi_config.ap.authmode = WIFI_AUTH_OPEN;
   }
   // set wifi mode, config, and start
   ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
   ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
   ESP_ERROR_CHECK(esp_wifi_start());

   ESP_LOGI(TAG,
            "wifi_init_softap finished. SSID:%s password:%s channel:%d",
            EXAMPLE_ESP_WIFI_SSID,
            EXAMPLE_ESP_WIFI_PASS,
            EXAMPLE_ESP_WIFI_CHANNEL);
}

/**
 * @brief function to start mDNS service
 */
void start_mdns_service() {
   //initialize mDNS service
   esp_err_t err = mdns_init();
   if(err) {
      printf("MDNS Init failed: %d\n", err);
      return;
   }

   //set hostname
   mdns_hostname_set("ocd-device");
   //set default instance
   mdns_instance_name_set(MDNS_INSTANCE);
}
