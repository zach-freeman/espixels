#include "NetworkStore.hpp"
#include "EspWifi.hpp"
#include "esp_log.h"
#include "TaskBase.hpp"
#include "TaskConfig.hpp"

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

//==============================================================================
// Public function definitions
//==============================================================================

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
NetworkStore::NetworkStore()
{
    StartWifiInfoTask();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::ProcessAction(NetworkAction &action)
{
    auto &actionTypes = action.GetTypes();
    ESP_LOGE(NETWORK_STORE_TAG, "got an action");
    for (auto &type : actionTypes)
    {
        switch (type)
        {
            case NetworkActionType::ClientConnect:
                ESP_LOGE(NETWORK_STORE_TAG, "got a client connect action");
                mWifi.SetSsid((char *)action.GetSsid());
                mWifi.SetPassword((char *)action.GetPassphrase());
                mWifi.SetMode(EspWifi::WifiMode::Station);
                mWifi.Start();
                break;
            case NetworkActionType::WifiConnected:
                ESP_LOGE(NETWORK_STORE_TAG, "WifiConnected");
                break;
            case NetworkActionType::GotIP:
                ESP_LOGE(NETWORK_STORE_TAG, "got an ip address");
                break;
            default:
                break;
        }
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::ProcessAction(TimeAction &timeAction)
{

}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::ProcessAction(UIAction &uiAction)
{
    
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void WifiInfoTaskRunner(void *params)
{
    static_cast<NetworkStore *>(params)->WifiInfoTask();
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::StartWifiInfoTask()
{
    if (xTaskCreate(WifiInfoTaskRunner,
        "wifi_info",
        WIFI_INFO_TASK_SIZE,
        this,
        WIFI_INFO_TASK_PRIORITY,
        &mWifiInfoTaskHandle) != pdPASS)
    {
        ESP_LOGE(NETWORK_STORE_TAG, "Unable to start wifi info task");
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::WifiInfoTask()
{
    while (FOREVER())
    {
        vTaskDelay(pdMS_TO_TICKS(WIFI_INFO_TASK_DELAY_MILLIS));
        mWifi.ShowIpAddress();
        mWifi.ShowHostname();
    }
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::TaskWifiInfoStopTask() const
{
    vTaskSuspend(mWifiInfoTaskHandle);
}

