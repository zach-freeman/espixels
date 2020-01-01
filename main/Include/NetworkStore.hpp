#ifndef NETWORK_STORE_HPP
#define NETWORK_STORE_HPP

#include <stdint.h>
#include "Store.hpp"
#include "EspWifi.hpp"

//==============================================================================
// Public defines and constants
//==============================================================================

//==============================================================================
// Public typedefs
//==============================================================================

//==============================================================================
// Public variables
//==============================================================================
static constexpr const char *   NETWORK_STORE_TAG{"Network Store"};
//==============================================================================
// Public function prototypes
//==============================================================================

class NetworkStore : public Store
{
public:
    NetworkStore();
    virtual ~NetworkStore() = default;
    void ProcessAction(NetworkAction &action) override;
    void ProcessAction(TimeAction &action) override;
    void ProcessAction(UIAction &action) override;
    void StartWifiInfoTask();
    void TaskWifiInfoStopTask() const;
    void WifiInfoTask();

private:
    //==========================================================================
    // Private constants
    //==========================================================================
    static constexpr uint32_t WIFI_INFO_TASK_DELAY_MILLIS{4000};
    //==========================================================================
    // Private data members
    //==========================================================================
    EspWifi mWifi;
    TaskHandle_t mWifiInfoTaskHandle{};

};

#endif    // NETWORK_STORE_HPP