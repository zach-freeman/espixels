#include "TimeStore.hpp"
#include "esp_log.h"
#include "esp_sntp.h"

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
void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGE(TIME_STORE_TAG, "Notification of a time synch event");
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
TimeStore::TimeStore()
{
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void TimeStore::ProcessAction(TimeAction &action)
{
    auto &actionTypes = action.GetTypes();
    ESP_LOGE(TIME_STORE_TAG, "got an action");
    for (auto &type : actionTypes)
    {
        switch (type)
        {
            case TimeActionType::InitializeTime:
                ESP_LOGE(TIME_STORE_TAG, "got an initialize time action");
                InitializeSntp();
                break;
            default:
                break;
        }
    }
}

void TimeStore::InitializeSntp()
{
    ESP_LOGE(TIME_STORE_TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_set_time_sync_notification_cb(time_sync_notification_cb);
}

