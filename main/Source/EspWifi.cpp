//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#include "EspWifi.hpp"

#include <cstring>

#include "Dispatcher.hpp"
#include "esp_event.h"
#include "esp_log.h"
extern "C" {
    #include "esp_wifi.h"
}

#include "esp_netif.h"
#include "TimeAction.hpp"
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
        esp_wifi_connect();
        ESP_LOGE(ESP_WIFI_TAG, "WIFI_EVENT_STA_START");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        ESP_LOGE(ESP_WIFI_TAG, "WIFI_EVENT_STA_DISCONNECTED");
        networkAction.AddType(NetworkActionType::WifiDisconnected);
        Dispatcher::GetInstance().SendAction(networkAction, portMAX_DELAY);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        ESP_LOGE(ESP_WIFI_TAG, "WIFI_EVENT_STA_CONNECTED");
        networkAction.AddType(NetworkActionType::WifiConnected);
        Dispatcher::GetInstance().SendAction(networkAction);       
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
        TimeAction timeAction(TimeActionType::None);
        timeAction.AddType(TimeActionType::InitializeTime);
        Dispatcher::GetInstance().SendAction(timeAction, portMAX_DELAY);
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
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
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

void EspWifi::ShowHostname()
{        
    esp_err_t err;
    const char *name;
    if ((err = tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_STA, &name)) != ESP_OK) {
        ESP_LOGE(ESP_WIFI_TAG, "Err Get Hostname: %s\n", esp_err_to_name(err));
    } else {
        ESP_LOGE(ESP_WIFI_TAG, "Hostname: %s", (name == NULL ? "<None>" : name));
    } 
}

void EspWifi::ShowIpAddress()
{
    tcpip_adapter_ip_info_t ipInfo;
    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
    ESP_LOGE(ESP_WIFI_TAG, "Got IP: " IPSTR "\n", IP2STR(&ipInfo.ip));
}

void EspWifi::Start()
{
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    esp_wifi_set_ps(WIFI_PS_NONE);
    wifi_config_t wifiConfig{};
    strncpy((char *)wifiConfig.sta.ssid, mSsid, MAX_SSID_LEN);
    strncpy((char *)wifiConfig.sta.password, mPassword, MAX_PASSWORD_LEN);
    ESP_LOGE(ESP_WIFI_TAG, "ssid is %s", mSsid);
    ESP_LOGE(ESP_WIFI_TAG, "passpharase is %s", mPassword);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifiConfig));
    mShouldConnect = true;
    ESP_ERROR_CHECK(esp_wifi_start());
}

void EspWifi::Stop()
{
    ESP_LOGE(ESP_WIFI_TAG, "Stop event received");
    mShouldConnect = false;
    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler);
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler);
    esp_wifi_stop();
}

