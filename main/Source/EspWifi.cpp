//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#include "EspWifi.hpp"

#include <cstring>

#include "Dispatcher.hpp"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
//==============================================================================
// Private defines and constants
//==============================================================================

//==============================================================================
// Private typedefs
//==============================================================================

//==============================================================================
// Private variables
//==============================================================================
//==============================================================================
// Private function definitions
//==============================================================================
static void event_handler(void * arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    ESP_LOGE(ESP_WIFI_TAG, "event handler");
    NetworkAction   networkAction(NetworkActionType::None);
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        //const char hostname[32] = "cmonman";
        //tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, hostname);
        //ESP_LOGI(ESP_WIFI_TAG, "hostname is esp32zach");
        esp_wifi_connect();
        ESP_LOGE(ESP_WIFI_TAG, "WIFI_EVENT_STA_START");
        networkAction.AddType(NetworkActionType::WifiConnected);
        Dispatcher::GetInstance().SendAction(networkAction);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGE(ESP_WIFI_TAG, "WIFI_EVENT_STA_DISCONNECTED");
        networkAction.AddType(NetworkActionType::WifiDisconnected);
        Dispatcher::GetInstance().SendAction(networkAction, portMAX_DELAY);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        ESP_LOGE(ESP_WIFI_TAG, "IP_EVENT_STA_GOT_IP");
        ESP_LOGE(ESP_WIFI_TAG, "Got IP: " IPSTR "\n", IP2STR(&event->ip_info.ip));
        ESP_LOGE(ESP_WIFI_TAG, "Got GW: " IPSTR "\n", IP2STR(&event->ip_info.gw));
        networkAction.AddType(NetworkActionType::GotIP);
        networkAction.SetIpAddress(event->ip_info.ip.addr);
        Dispatcher::GetInstance().SendAction(networkAction, portMAX_DELAY);
    }
    else
    {
        if (event_base == IP_EVENT)
        {
            ESP_LOGE(ESP_WIFI_TAG, "IP_EVENT");
        }
        else if (event_base == WIFI_EVENT)
        {
            ESP_LOGE(ESP_WIFI_TAG, "WIFI_EVENT");
            if (event_id == WIFI_EVENT_STA_CONNECTED)
            {
                ESP_LOGE(ESP_WIFI_TAG, "CONNECTED");
            }
        }
        
    }
    
}
//==============================================================================
// Public function definitions
//==============================================================================
EspWifi::EspWifi()
{
    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    // set default mode to station so that scanning can work
    wifi_config_t wifiConfig{};
    wifiConfig.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    wifiConfig.sta.scan_method = WIFI_FAST_SCAN;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void EspWifi::SetSsid(const char *ssid)
{
    strncpy(mSsid, ssid, MAX_SSID_LEN);
}

void EspWifi::SetPassword(const char *password)
{
    strncpy(mPassword, password, MAX_PASSWORD_LEN);
}

void EspWifi::SetMode(WifiMode mode)
{
    mWifiMode = mode;
}

void EspWifi::Start()
{
    esp_wifi_set_ps(WIFI_PS_NONE);
    wifi_config_t wifiConfig{};
    if (mWifiMode == WifiMode::Station)
    {
        strncpy((char *)wifiConfig.sta.ssid, mSsid, MAX_SSID_LEN);
        strncpy((char *)wifiConfig.sta.password, mPassword, MAX_PASSWORD_LEN);
        ESP_LOGE(ESP_WIFI_TAG, "ssid is %s", mSsid);
        ESP_LOGE(ESP_WIFI_TAG, "passpharase is %s", mPassword);
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig));
        mShouldConnect = true;
    }
    else if (mWifiMode == WifiMode::AccessPoint)
    {
        strncpy((char *)wifiConfig.ap.ssid, mSsid, MAX_SSID_LEN);
        wifiConfig.ap.ssid_len = strlen(mSsid);
        strncpy((char *)wifiConfig.ap.password, mPassword, MAX_PASSWORD_LEN);
        wifiConfig.ap.authmode       = WIFI_AUTH_WPA_WPA2_PSK;
        wifiConfig.ap.max_connection = MAX_AP_CONNECTIONS;
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifiConfig));
    }
    ESP_ERROR_CHECK(esp_wifi_start());
}

void EspWifi::Stop()
{
    mShouldConnect = false;
    esp_wifi_stop();
}

