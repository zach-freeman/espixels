#include "NetworkStore.hpp"
#include "EspWifi.hpp"
#include "esp_log.h"

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
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void NetworkStore::ProcessAction(NetworkAction &action)
{
    auto &actionTypes = action.GetTypes();
    ESP_LOGE("NetworkStore", "got an action");
    for (auto &type : actionTypes)
    {
        switch (type)
        {
            case NetworkActionType::ClientConnect:
                ESP_LOGE("NetworkStore", "got a client connect action");
                mWifi.Stop();
                mWifi.SetSsid((char *)action.GetSsid());
                mWifi.SetPassword((char *)action.GetPassphrase());
                mWifi.SetMode(EspWifi::WifiMode::Station);
                mWifi.Start();
                break;
            case NetworkActionType::WifiConnected:
                ESP_LOGE("NetworkStore", "WifiConnected");
                break;
            case NetworkActionType::GotIP:
                ESP_LOGE("NetworkStore", "got an ip address");
                break;
            default:
                break;
        }
    }
}

