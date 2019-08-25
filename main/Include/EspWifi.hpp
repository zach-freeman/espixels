//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef ESP_WIFI_INCLUDED
#define ESP_WIFI_INCLUDED

//==============================================================================
// Public defines and constants
//==============================================================================
#include <cstdint>

#include "NetworkAction.hpp"
#include "Wifi.hpp"

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================

//==============================================================================
// Public function prototypes
//==============================================================================
class EspWifi : public Wifi
{
public:
    EspWifi();
    void SetSsid(const char *ssid) override;
    void SetPassword(const char *password) override;
    void SetMode(WifiMode mode) override;
    void Start() override;
    void Stop() override;
    void WifiEventHandler(void *event);    // override;

private:
    static constexpr const char *   ESP_WIFI_TAG{"ESP Wifi"};
    static constexpr const uint32_t MAX_AP_CONNECTIONS{4};
    static constexpr uint32_t       MAX_SSID_LEN{32};
    static constexpr uint32_t       MAX_PASSWORD_LEN{64};

    int32_t  mStationCount{0};
    char     mSsid[MAX_SSID_LEN];
    char     mPassword[MAX_PASSWORD_LEN];
    WifiMode mWifiMode;
    bool     mShouldConnect{false};
};
#endif    // ESP_WIFI_INCLUDED