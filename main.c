#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_err.h"
#include "wifi_provisioning/manager.h"
#include "wifi_provisioning/scheme_ble.h"
#include "esp_bt.h"
#include "esp_netif.h"

static const char *TAG = "app";

void app_main(void)
{
    // Initialize NVS (Non-Volatile Storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the ESP event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize the TCP/IP stack and Wi-Fi driver
    ESP_ERROR_CHECK(esp_netif_init());

    // Create default Wi-Fi station interface
    esp_netif_create_default_wifi_sta();

    // Set up Wi-Fi initialization
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Initialize the provisioning manager for BLE-based provisioning
    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE
    };
    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

    bool provisioned = false;
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    // Check if device is provisioned
    if (!provisioned) {
        ESP_LOGI(TAG, "Starting provisioning");

        // Configure provisioning parameters
        const char *service_name = "PROV_ESP32";
        const char *pop = "abcd1234";  // Proof of possession code
        const char *service_key = NULL; // Set to NULL for open network
        wifi_prov_security_t security = WIFI_PROV_SECURITY_1;

        // Start provisioning process
        ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop, service_name, service_key));

        // Wait for provisioning to finish
        wifi_prov_mgr_wait();
    } else {
        ESP_LOGI(TAG, "Already provisioned, starting Wi-Fi");

        // Set Wi-Fi mode to STA (Station mode)
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());
    }

    // Cleanup the provisioning manager after completion
    wifi_prov_mgr_deinit();
}
