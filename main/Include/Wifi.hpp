//==============================================================================
// Copyright © 2019 Sparkwing
//==============================================================================
#ifndef WIFI_INCLUDED
#define WIFI_INCLUDED


//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================

//==============================================================================
// Public function prototypes
//==============================================================================
// TODO: add this backwhen esp supports gcc8
// class Wifi :  public WifiEventInterface
class Wifi
{
public:
    enum class WifiMode
    {
        Station,
        AccessPoint
    };
    virtual void SetSsid(const char *ssid)         = 0;
    virtual void SetPassword(const char *password) = 0;
    virtual void SetMode(WifiMode mode)            = 0;
    virtual void Start()                           = 0;
    virtual void Stop()                            = 0;
};
#endif    // WIFI_INCLUDED